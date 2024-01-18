cmake_minimum_required(VERSION 3.27)
project(fisheye_camera)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(fisheye_camera main.cpp fisheye_camera.cpp)
find_package(spdlog REQUIRED)
target_link_libraries(
        fisheye_camera spdlog::spdlog systemd ${OpenCV_LIBS})
