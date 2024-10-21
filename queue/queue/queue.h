//
// Created by kaylor chen on 2023/9/24.
//

#ifndef QUEUE_QUEUE_H
#define QUEUE_QUEUE_H

template<class T>
class Queue {
public:
    virtual int Size() = 0;

    virtual bool IsEmpty() = 0;

    virtual void Print(){};

    //入队操作
    virtual void Enqueue(T& num) = 0;

    //出队操作
    virtual void Dequeue() = 0;

    //获得队首元素
    virtual T *Front() = 0;

    //获取队尾元素
    virtual T *Rear() = 0;
};

#endif //QUEUE_QUEUE_H
