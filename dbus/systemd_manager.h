//
// Created by kaylor on 6/20/25.
//

#ifndef DBUS_MANAGER_H
#define DBUS_MANAGER_H

#include <dbus/dbus.h>
class SystemdManager {
 public:
  SystemdManager();
  bool CallMethod(const std::string& path, const std::string& interface,
                  const std::string& method, const char* arg_type = nullptr,
                  void* arg = nullptr);
  std::string GetProperty(const std::string& unit_path,
                          const std::string& property);

 private:
  DBusConnection* conn_;
};

#endif  // DBUS_MANAGER_H
