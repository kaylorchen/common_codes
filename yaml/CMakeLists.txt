cmake_minimum_required(VERSION 3.27)
project(yaml)

set(CMAKE_CXX_STANDARD 17)
find_package(yaml-cpp REQUIRED)

add_executable(yaml main.cpp)
target_link_libraries(yaml ${YAML_CPP_LIBRARIES})
