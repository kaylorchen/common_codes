//
// Created by kaylor chen on 2023/10/31.
//

#include "pid.h"

Pid::Pid(double kp, double ki, double kd) : kp_(kp), ki_(ki), kd_(kd), error_{0.0, 0.0, 0.0}, last_output_(0.0) {}

double Pid::PidCalc(const double error, const double sample_time) {
  try {
    if (sample_time == 0.0) {
      throw (sample_time);
    }
    error_[0] = error;
    double delta_output = kp_ * (
        (1 + ki_ * sample_time + kd_ / sample_time) * error_[0]
            - (1 + 2 * kd_ / sample_time) * error_[1]
            + kd_ / sample_time * error_[2]
    );
    // 误差滚动替换
    error_[2] = error_[1];
    error_[1] = error_[0];
    last_output_ += delta_output;
    return delta_output;
  } catch (double time) {
    std::cerr << "Error: sample time is " << time << std::endl;
    return 0;
  }
}

void Pid::UpdatePidParameters(double kp, double ki, double kd) {
  kd_ = kd;
  ki_ = ki;
  kp_ = kp;
}