cmake_minimum_required(VERSION 3.16)
project(pcl)

set(CMAKE_CXX_STANDARD 17)

find_package(PCL 1.10 REQUIRED)
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS})
add_definitions(${PCL_DEFINITIONS})

add_executable(pcl_rotate pcl_rotate.cpp main.cpp)
target_link_libraries(pcl_rotate ${PCL_LIBRARIES})
