cmake_minimum_required(VERSION 3.28)
project(camera_with_opencv)

set(CMAKE_CXX_STANDARD 17)
find_package(yaml-cpp REQUIRED)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(camera_with_opencv main.cpp)
target_link_libraries(camera_with_opencv ${YAML_CPP_LIBRARIES} ${OpenCV_LIBS})
