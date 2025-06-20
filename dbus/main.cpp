#include "kaylordut/log/logger.h"
#include "systemd_manager.h"
#include "thread"

int main(int argc, char **argv) {
  KAYLORDUT_LOG_INFO("Dbus demo");
  SystemdManager systemd_manager;
  std::string service = "print_test";
  systemd_manager.Start(service);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  systemd_manager.Restart(service);
  std::this_thread::sleep_for(std::chrono::seconds(1));
  systemd_manager.Stop(service);
  return 0;
}