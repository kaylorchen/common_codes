//
// Created by kaylor on 6/20/25.
//

#include "systemd_manager.h"

#include "kaylordut/log/logger.h"

#define CHECK_DBUS_ERROR(err, msg)                   \
  if (dbus_error_is_set(&err)) {                     \
    KAYLORDUT_LOG_ERROR("{}: {}", msg, err.message); \
    dbus_error_free(&err);                           \
    return false;                                    \
  }

bool SystemdManager::CallMethod(const std::string& path,
                                const std::string& interface,
                                const std::string& method, const char* arg_type,
                                void* arg) {
  DBusError err;
  dbus_error_init(&err);

  DBusMessage* msg =
      dbus_message_new_method_call("org.freedesktop.systemd1", path.c_str(),
                                   interface.c_str(), method.c_str());
  if (arg_type && arg) {
    dbus_message_append_args(msg, dbus_message_type_from_string(arg_type), arg,
                             DBUS_TYPE_INVALID);
  }
  DBusMessage* reply =
      dbus_connection_send_with_reply_and_block(conn_, msg, 5000, &err);
  CHECK_DBUS_ERROR(err, "DBus call failed");
  bool success = (dbus_message_get_type(reply) != DBUS_MESSAGE_TYPE_ERROR);
  if (!success) {
    KAYLORDUT_LOG_ERROR("Operation failed: {}",
                        dbus_message_get_error_name(reply));
  }
  dbus_message_unref(msg);
  dbus_message_unref(reply);
  return success;
}

std::string SystemdManager::GetProperty(const std::string& unit_path,
                                        const std::string& property) {
  DBusError err;
  dbus_error_init(&err);

  DBusMessage* msg = dbus_message_new_method_call(
      "org.freedesktop.systemd1", unit_path.c_str(),
      "org.freedesktop.DBus.Properties", "Get");

  const char* interface = "org.freedesktop.systemd1.Unit";
  const char* prop = property.c_str();

  dbus_message_append_args(msg, DBUS_TYPE_STRING, &interface, DBUS_TYPE_STRING,
                           &prop, DBUS_TYPE_INVALID);

  DBusMessage* reply =
      dbus_connection_send_with_reply_and_block(conn_, msg, 5000, &err);

  CHECK_DBUS_ERROR(err, "Failed to get property");

  std::string result;
  DBusMessageIter iter, variant_iter;
  dbus_message_iter_init(reply, &iter);
  dbus_message_iter_recurse(&iter, &variant_iter);

  const char* value;
  dbus_message_iter_get_basic(&variant_iter, &value);
  result = value;

  dbus_message_unref(msg);
  dbus_message_unref(reply);

  return result;
}

SystemdManager::SystemdManager() {
  DBusError err;
  dbus_error_init(&err);
  conn_ = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
  if (dbus_error_is_set(&err)) {
    std::cerr << "Failed to connect to system bus: " << err.message
              << std::endl;
    dbus_error_free(&err);
    exit(1);
  }
}

SystemdManager::~SystemdManager() {
  if (conn_) {
    dbus_connection_unref(conn_);
  }
}

std::string SystemdManager::GetUnitPath(const std::string& service_name) {
  return "/org/freedesktop/systemd1/unit/" +
         (service_name.find('.') != std::string::npos
              ? service_name
              : service_name + ".service");
}

bool SystemdManager::StartService(const std::string& service_name) {
  std::string unit_path = GetUnitPath(service_name);
  const char* mode = "replace";
  return CallMethod(unit_path, "org.freedesktop.systemd1.Unit", "Start",
                    DBUS_TYPE_STRING_AS_STRING, &mode);
}

bool SystemdManager::StopService(const std::string& service_name) {
  std::string unit_path = GetUnitPath(service_name);
  const char* mode = "replace";
  return CallMethod(unit_path, "org.freedesktop.systemd1.Unit", "Stop",
                    DBUS_TYPE_STRING_AS_STRING, &mode);
}

bool SystemdManager::RestartService(const std::string& service_name) {
  std::string unit_path = GetUnitPath(service_name);
  const char* mode = "replace";
  return CallMethod(unit_path, "org.freedesktop.systemd1.Unit", "Restart",
                    DBUS_TYPE_STRING_AS_STRING, &mode);
}

bool SystemdManager::EnableService(const std::string& service_name) {
  std::vector<const char*> units = {service_name.c_str()};
  const char* mode = "replace";
  const char* enable = "1";

  DBusMessage* msg = dbus_message_new_method_call(
      "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
      "org.freedesktop.systemd1.Manager", "EnableUnitFiles");

  dbus_message_append_args(msg, DBUS_TYPE_ARRAY, DBUS_TYPE_STRING_AS_STRING,
                           &units, 1, DBUS_TYPE_BOOLEAN, &enable,
                           DBUS_TYPE_STRING, &mode, DBUS_TYPE_INVALID);

  DBusError err;
  dbus_error_init(&err);

  DBusMessage* reply =
      dbus_connection_send_with_reply_and_block(conn_, msg, 5000, &err);

  CHECK_DBUS_ERROR(err, "Failed to enable service");

  bool success = (dbus_message_get_type(reply) != DBUS_MESSAGE_TYPE_ERROR);

  if (success) {
    // Reload systemd configuration
    success = CallMethod("/org/freedesktop/systemd1",
                         "org.freedesktop.systemd1.Manager", "Reload");
  }

  dbus_message_unref(msg);
  dbus_message_unref(reply);

  return success;
}

bool SystemdManager::DisableService(const std::string& service_name) {
  std::vector<const char*> units = {service_name.c_str()};

  DBusMessage* msg = dbus_message_new_method_call(
      "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
      "org.freedesktop.systemd1.Manager", "DisableUnitFiles");

  dbus_message_append_args(msg, DBUS_TYPE_ARRAY, DBUS_TYPE_STRING_AS_STRING,
                           &units, 1, DBUS_TYPE_BOOLEAN, "0",
                           DBUS_TYPE_INVALID);

  DBusError err;
  dbus_error_init(&err);

  DBusMessage* reply =
      dbus_connection_send_with_reply_and_block(conn_, msg, 5000, &err);

  CHECK_DBUS_ERROR(err, "Failed to disable service");

  bool success = (dbus_message_get_type(reply) != DBUS_MESSAGE_TYPE_ERROR);

  if (success) {
    // Reload systemd configuration
    success = CallMethod("/org/freedesktop/systemd1",
                         "org.freedesktop.systemd1.Manager", "Reload");
  }

  dbus_message_unref(msg);
  dbus_message_unref(reply);

  return success;
}

std::map<std::string, std::string> SystemdManager::GetserviceStatus(
    const std::string& service_name) {
  std::string unit_path = GetUnitPath(service_name);
  std::map<std::string, std::string> status;
  status["ActiveState"] = GetProperty(unit_path, "ActiveState");
  status["SubState"] = GetProperty(unit_path, "SubState");
  status["UnitFileState"] = GetProperty(unit_path, "UnitFileState");
  return status;
}
