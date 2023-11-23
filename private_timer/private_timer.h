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

  template <typename T, typename Rep, typename Period>
  void start(const std::chrono::duration<Rep, Period>& interval, T callback) {
    m_isRunning = true;
    std::thread([=]() {
      while (m_isRunning) {
        auto start = std::chrono::system_clock::now();
        callback();
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;

        if (elapsed < interval) {
          std::this_thread::sleep_for(interval - elapsed);
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
