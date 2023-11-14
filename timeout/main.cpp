//
// Created by kaylor chen on 2023/11/14.
//
#include "timeout.h"
#include <iostream>
#include <chrono>
#include <iomanip>
#include <thread>

int main() {
  Timeout timeout(5000);
  while(!timeout.isTimeout()){
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    struct tm* time_info = std::localtime(&now_time);
    std::cout << "当前时刻为：";
    std::cout << std::put_time(time_info, "%Y-%m-%d %H:%M:%S") << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
  return 0;
}
