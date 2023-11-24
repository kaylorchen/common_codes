//
// Created by kaylor chen on 2023/11/14.
//

#pragma once
#include <chrono>
template<typename T>
class Timeout {
 public:
  Timeout(const T &timeout) : timeout_(timeout) {
    start_ = std::chrono::steady_clock::now();
  }
  bool isTimeout() {
    auto elapsed = std::chrono::duration_cast<T>(std::chrono::steady_clock::now() - start_);
    if (elapsed >= timeout_) { return true; }
    return false;
  }
  void set_timeout(const T &timeout) {
    timeout_ = timeout;
    start_ = std::chrono::steady_clock::now();
  }
 private:
  T timeout_;
  std::chrono::steady_clock::time_point start_;
};
