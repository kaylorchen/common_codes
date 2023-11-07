//
// Created by kaylor chen on 2023/11/7.
//

#ifndef COMMON_CODES_FILTERS_FIRST_ORDER_H_
#define COMMON_CODES_FILTERS_FIRST_ORDER_H_

class FirstOrder {
  FirstOrder(double sample_period, double time_constant) {
    sample_period_ = sample_period;
    time_constant_ = time_constant;
    lamda_ = sample_period / time_constant;
    output_ = 0;
  }
  double Filter(const double &input) {
    output_ = lamda_ * input + (1 - lamda_) * output_;
    return output_;
  }
 private:
  double sample_period_;
  double time_constant_;
  double lamda_;
  double output_;
};

#endif //COMMON_CODES_FILTERS_FIRST_ORDER_H_
