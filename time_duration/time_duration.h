//
// Created by kaylor chen on 2023/11/21.
//

#ifndef TIME_DURATION__TIME_DURATION_H_
#define TIME_DURATION__TIME_DURATION_H_
#include "chrono"
class TimeDuration {
 public:
  TimeDuration() { time_point_ = std::chrono::steady_clock::now(); }
  // 返回距离上次调用该方法的时间间隔，单位为 ms
  float DurationSinceLastTime() {
    auto now = std::chrono::steady_clock::now();
    float duration = std::chrono::duration_cast<std::chrono::microseconds>(now - time_point_).count() / 1000.0f;
    time_point_ = now;
    return duration;
  }
 private:
  std::chrono::steady_clock::time_point time_point_;
};

#endif //TIME_DURATION__TIME_DURATION_H_
