cmake_minimum_required(VERSION 3.22)
project(opencv_reshape)

set(CMAKE_CXX_STANDARD 17)


SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -Wno-unused-parameter -O3 -g -Wall -fopenmp -lgmp")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS}  -Wno-unused-parameter -O3 -g -Wall -fopenmp -lgmp")
# 寻找OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
find_package(spdlog REQUIRED)

add_executable(opencv_reshape main.cpp)
target_link_libraries(opencv_reshape ${OpenCV_LIBS} spdlog::spdlog systemd)
add_executable(mul mul.cpp)
target_link_libraries(mul ${OpenCV_LIBS} spdlog::spdlog systemd)
add_executable(blob blob.cpp)
target_link_libraries(blob ${OpenCV_LIBS} spdlog::spdlog systemd)
