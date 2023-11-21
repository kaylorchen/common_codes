//
// Created by kaylor chen on 2023/11/21.
//
#include <unistd.h>
#include "iostream"
#include "time_duration.h"
int main(void) {
  TimeDuration time_duration;
  int i = 10;
  while (i--) {
    sleep(1);
    std::cout << "i = " << i << ", duration is " << time_duration.DurationSinceLastTime() << "ms\n";
  }
  return 0;
}