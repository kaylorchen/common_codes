#include <iostream>
#include "queue/loopqueue.h"
#include "queue/fixedloopqueue.h"
#include "memory"
class Kaylor {
 public:
  int a_;
  std::shared_ptr<int> p_;
 public:
  Kaylor &operator=(const Kaylor &s) {
//    std:cout <<"运算符重载"<< std::endl;
    this->a_ = s.a_;
    this->p_ = s.p_;
//    std::cout << "****" << this->p_.use_count() << "  " << s.p_.use_count() <<std::endl;
    return *this;
  }
};
int main() {
  std::shared_ptr<LoopQueue<Kaylor>> loop_queue = std::make_shared<LoopQueue<Kaylor>>(5);
  for (int i = 0; i < 5; ++i) {
    Kaylor kaylor;
    kaylor.a_ = i;
    kaylor.p_ = std::make_shared<int>(i + 1);
    loop_queue->Enqueue(kaylor);
  }

  auto fix_queue = std::make_shared<FixedLoopQueue<Kaylor>>(5);
  for (int i = 0; i < 5; ++i) {
    Kaylor kaylor;
    kaylor.a_ = i + 1;
    kaylor.p_ = std::make_shared<int>(i + 2);
    fix_queue->Enqueue(kaylor);
  }
  for (int i = 0; i < 5; ++i) {
    std::cout << "loop queue, a = " << loop_queue->Element(i)->a_ << ", *p = " << *(loop_queue->Element(i)->p_)
              << std::endl;
    std::cout << "fixed queue, a = " << fix_queue->Element(i)->a_ << ", *p = " << *(fix_queue->Element(i)->p_)
              << std::endl;
    std::cout << "************"<< std::endl;
  }
  return 0;
}
