//
// Created by kaylor on 6/20/25.
//

#include "systemd_manager.h"

#include "kaylordut/log/logger.h"
#include "systemd/sd-bus.h"

int SystemdManager::ControlService(const std::string service,
                                   const char* method) {
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_message* reply = nullptr;
  sd_bus* bus = nullptr;

  // 1. 连接系统总线
  int ret = sd_bus_open_system(&bus);
  if (ret < 0) {
    std::cerr << "Bus connection error: " << strerror(-ret) << std::endl;
    return ret;
  }

  // 2. 构造 D-Bus 调用
  auto service_full_name = std::string(service)+".service";
  ret = sd_bus_call_method(bus,
                           "org.freedesktop.systemd1",          // 目标
                           "/org/freedesktop/systemd1",         // 路径
                           "org.freedesktop.systemd1.Manager",  // 接口
                           method,  // 方法: "StartUnit" 或 "StopUnit"
                           &error, &reply,
                           "ss",             // 参数类型: 两个字符串
                           service_full_name.c_str(),  // 服务名
                           "replace"         // 模式: 替换现有任务
  );

  // 3. 处理结果
  if (ret < 0) {
    std::cerr << method << " failed: " << error.message << std::endl;
  } else {
    std::cout << "Success: " << method << std::endl;
  }

  // 4. 清理资源
  sd_bus_error_free(&error);
  sd_bus_message_unref(reply);
  sd_bus_unref(bus);
  return ret;
}

int SystemdManager::Start(std::string service) {
  return ControlService(service.c_str(), "StartUnit");
}

int SystemdManager::Stop(std::string service) {
  return ControlService(service.c_str(), "StopUnit");
}

int SystemdManager::Restart(std::string service) {
  return ControlService(service.c_str(), "RestartUnit");
}
