cmake_minimum_required(VERSION 3.25)
project(opencv_mat)

set(CMAKE_CXX_STANDARD 17)

# 寻找OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(spdlog REQUIRED)

add_executable(opencv_mat main.cpp )
target_link_libraries(opencv_mat ${OpenCV_LIBS} spdlog::spdlog systemd)
