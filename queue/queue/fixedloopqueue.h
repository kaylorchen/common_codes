//
// Created by kaylor chen on 2023/9/24.
//

#ifndef QUEUE_FIXEDLOOPQUEUE_H_
#define QUEUE_FIXEDLOOPQUEUE_H_


#include "queue.h"
#include "iostream"
#include <mutex>

template<class T>
class FixedLoopQueue : public Queue<T> {
 public:
  FixedLoopQueue(int len) {
    T *p = new T[len];
    data_ = p;
    capacity_ = len;
    size_ = front_ = rear_ = 0;
  }

  int Size() {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    return size_;
  }

  bool IsEmpty() {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    return size_ == 0;
  }

  T *Element(unsigned int index) {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    if (index > size_ - 1) {
      std::cout << "error: queue index is out of bounds" << std::endl;
      return nullptr;
    }
    return &data_[(index + front_) % capacity_];
  }

  //入队操作
  void Enqueue(T& num) {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    data_[rear_] = num;
    /*定长可覆盖循环队列，size增长到容量上限就不再增长*/
    if(size_ < capacity_){
      ++size_;
    }
    rear_ = (rear_ + 1) % capacity_;
  }

  //出队操作
  void Dequeue() {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    if (size_ <= 0) {
      std::cerr << "Error: Dequeue is empty" << std::endl;
      return;
    }
    size_--;
    front_ = (front_ + 1) % capacity_;
  }

  //获得队首元素
  T *Front() {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    if (size_ <= 0) {
      std::cerr << "Error: Dequeue is empty" << std::endl;
      return nullptr;
    }
    return &data_[front_];
  }

  //获得队尾元素
  T *Rear() {
    std::lock_guard<std::mutex> lock(queue_mutex_);
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
  std::mutex queue_mutex_;
};

#endif //QUEUE_FIXEDLOOPQUEUE_H_
