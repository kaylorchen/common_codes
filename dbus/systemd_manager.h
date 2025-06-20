//
// Created by kaylor on 6/20/25.
//

#ifndef DBUS_MANAGER_H
#define DBUS_MANAGER_H

#include <string>

class SystemdManager {
 public:
  int Start(std::string service);
  int Stop(std::string service);
  int Restart(std::string service);

 private:
  int ControlService(const std::string service, const char* method);
};

#endif  // DBUS_MANAGER_H
