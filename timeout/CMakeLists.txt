cmake_minimum_required(VERSION 3.25)
project(timeout)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(timeout
        main.cpp)
