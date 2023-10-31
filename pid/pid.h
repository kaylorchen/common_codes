//
// Created by kaylor chen on 2023/10/31.
//

#ifndef PID__PID_H_
#define PID__PID_H_
#include "iostream"

class Pid {
 public:
  Pid(double kp, double ki, double kd);
  double PidCalc(const double error, const double sample_time);
  void UpdatePidParameters(double kp, double ki, double kd);
 private:
  double kp_;
  double ki_;
  double kd_;
  double error_[3];
  double last_output_;
};

#endif //PID__PID_H_
