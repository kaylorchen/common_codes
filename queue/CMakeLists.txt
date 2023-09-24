cmake_minimum_required(VERSION 3.25)
project(queue)

set(CMAKE_CXX_STANDARD 17)

add_executable(queue main.cpp queue/queue.h queue/loopqueue.h queue/fixedloopqueue.h)
