//
// Created by kaylor chen on 2023/11/6.
//

#ifndef COMMON_CODES_PRIVATE_TIMER_PRIVATE_TIMER_H_
#define COMMON_CODES_PRIVATE_TIMER_PRIVATE_TIMER_H_
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

class PrivateTimer {
 public:
  PrivateTimer() {}
  ~PrivateTimer() {}

  template <typename T> void start(int interval, T callback) {
    m_isRunning = true;
    std::thread([=]() {
      while (m_isRunning) {
        auto start = std::chrono::system_clock::now();
        callback();
        auto end = std::chrono::system_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if (elapsed.count() < interval) {
          std::this_thread::sleep_for(
              std::chrono::milliseconds(interval - elapsed.count()));
        } else {
          std::cerr << "Elapsed: " << elapsed.count() << "ms" << std::endl;
        }
      }
    }).detach();
  }

  void stop() { m_isRunning = false; }

 private:
  bool m_isRunning = false;
};
#endif //COMMON_CODES_PRIVATE_TIMER_PRIVATE_TIMER_H_
