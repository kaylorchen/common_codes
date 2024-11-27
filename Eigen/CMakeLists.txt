cmake_minimum_required(VERSION 3.28)
project(Eigen_Test)

set(CMAKE_CXX_STANDARD 17)
find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIRS})

add_executable(eigen_test main.cpp)
