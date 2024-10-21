//
// Created by kaylor chen on 2023/9/24.
// 如果是基础的数据类型，可以使用print函数，如果是类，需要重载输出。
//

#ifndef QUEUE_LOOPQUEUE_H
#define QUEUE_LOOPQUEUE_H

#include <mutex>
#include "queue.h"
#include "iostream"

using namespace std;

template<class T>
class LoopQueue : public Queue<T> {
 public:
  LoopQueue(int len) {
    std::lock_guard<std::mutex> lock(queue_mutex_);
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
    std::lock_guard<std::mutex> lock_guard(queue_mutex_);
    return size_ == 0;
  }

  T *Element(unsigned int index) {
    std::lock_guard<std::mutex> lock_guard(queue_mutex_);
    if (index + 1 > size_) {
      std::cout << "error: queue index is out of bounds" << std::endl;
      return nullptr;
    }
    return &data_[(index + front_) % capacity_];
  }

  //入队操作
  void Enqueue(T& num) {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    if (size_ >= capacity_) {
      Reserve(2 * capacity_);
    }
    data_[rear_] = num;
    size_++;
    rear_ = (rear_ + 1) % capacity_;
  }

  //出队操作
  void Dequeue() {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    if (size_ <= 0) {
      std::cerr << "队列为空，出队操作失败！" << endl;
      return;
    }
    if (size_ <= capacity_ / 4) {
      Reserve(capacity_ / 2);
    }
    size_--;
    front_ = (front_ + 1) % capacity_;
  }

  //获得队首元素
  T *Front() {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    if (size_ <= 0) {
      std::cerr << "队列为空，操作失败！" << endl;
      return nullptr;
    }
    return &data_[front_];
  }

  //获得队尾元素
  T *Rear() {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    if (size_ <= 0) {
      std::cerr << "队列为空，操作失败！" << endl;
      return nullptr;
    }
    return &data_[(front_ + size_ - 1) % capacity_];
  }

 private:
  void Reserve(int len) {
    T *p = new T[len];
    if (size_ == 0) {}
    else {
      for (int i = 0; i < size_; ++i) {
        p[i] = data_[(i + front_) % capacity_];
      }
    }
    delete[] data_;
    data_ = p;
    capacity_ = len;
    front_ = 0;
    rear_ = size_;
  }
  T *data_;
  int capacity_;
  int size_;
  int front_;
  int rear_;
  std::mutex queue_mutex_;
};

#endif //QUEUE_LOOPQUEUE_H
