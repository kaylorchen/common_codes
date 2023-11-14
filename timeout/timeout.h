//
// Created by kaylor chen on 2023/11/14.
//

#ifndef TIMEOUT__TIMEOUT_H_
#define TIMEOUT__TIMEOUT_H_
#include <chrono>
class Timeout {
 public:
  Timeout(unsigned long int timeout) : timeout_(timeout) {
    start_ = std::chrono::steady_clock::now();
  }
  bool isTimeout() {
    end_ = std::chrono::steady_clock::now();
    auto elapsed =
        std::chrono::duration_cast<std::chrono::milliseconds>(end_ - start_);
    if (elapsed.count() >= timeout_) { return true; }
    return false;
  }
  void set_timeout(unsigned long int timeout){
    timeout_ = timeout;
    start_ = std::chrono::steady_clock::now();
  }
 private:
  unsigned long int timeout_;
  std::chrono::steady_clock::time_point start_;
  std::chrono::steady_clock::time_point end_;
};

#endif //TIMEOUT__TIMEOUT_H_
