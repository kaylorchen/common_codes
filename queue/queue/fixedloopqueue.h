//
// Created by kaylor chen on 2023/9/24.
//

#ifndef QUEUE_FIXEDLOOPQUEUE_H_
#define QUEUE_FIXEDLOOPQUEUE_H_


#include "queue.h"
#include "iostream"

template<class T>
class FixedLoopQueue : public Queue<T> {
 public:
  FixedLoopQueue(int len) {
    T *p = new T[len];
    data_ = p;
    capacity_ = len;
    size_ = front_ = rear_ = 0;
  }

  int size() {
    return size_;
  }

  bool IsEmpty() {
    return size_ == 0;
  }

  T *Element(unsigned int index) {
    if (index > size_ - 1) {
      std::cout << "error: queue index is out of bounds" << std::endl;
      return nullptr;
    }
    return &data_[(index + front_) % capacity_];
  }

  //入队操作
  void Enqueue(T& num) {
    data_[rear_] = num;
    /*定长可覆盖循环队列，size增长到容量上限就不再增长*/
    if(size_ < capacity_){
      ++size_;
    }
    rear_ = (rear_ + 1) % capacity_;
  }

  //出队操作
  void Dequeue() {
    if (size_ <= 0) {
      std::cerr << "Error: Dequeue is empty" << std::endl;
      return;
    }
    size_--;
    front_ = (front_ + 1) % capacity_;
  }

  //获得队首元素
  T *Front() {
    if (size_ <= 0) {
      std::cerr << "Error: Dequeue is empty" << std::endl;
      return nullptr;
    }
    return &data_[front_];
  }

  //获得队尾元素
  T *Rear() {
    if (size_ <= 0) {
      std::cerr << "Error: Dequeue is empty" << std::endl;
      return nullptr;
    }
    return &data_[(front_ + size_ - 1) % capacity_];
  }

 private:
  T *data_;
  int capacity_;
  int size_;
  int front_;
  int rear_;
};

#endif //QUEUE_FIXEDLOOPQUEUE_H_
