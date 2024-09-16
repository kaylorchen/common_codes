cmake_minimum_required(VERSION 3.28)
project(matmul)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-O3")

find_package(kaylordut REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(matmul
        matmul.cpp)
target_link_libraries(matmul ${kaylordut_LIBS} ${OpenCV_LIBS})
