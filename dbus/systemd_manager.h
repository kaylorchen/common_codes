//
// Created by kaylor on 6/20/25.
//

#ifndef DBUS_MANAGER_H
#define DBUS_MANAGER_H

#include <dbus/dbus.h>

#include <map>
#include <string>

class SystemdManager {
 public:
  SystemdManager();
  ~SystemdManager();
  std::string GetUnitPath(const std::string& service_name);
  bool StartService(const std::string& service_name);
  bool StopService(const std::string& service_name);
  bool RestartService(const std::string& service_name);
  bool EnableService(const std::string& service_name);
  bool DisableService(const std::string& service_name);
  std::map<std::string, std::string> GetserviceStatus(
      const std::string& service_name);

 private:
  DBusConnection* conn_;
  bool CallMethod(const std::string& path, const std::string& interface,
                  const std::string& method, const char* arg_type = nullptr,
                  void* arg = nullptr);
  std::string GetProperty(const std::string& unit_path,
                          const std::string& property);
};

#endif  // DBUS_MANAGER_H
