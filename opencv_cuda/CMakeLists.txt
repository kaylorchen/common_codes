cmake_minimum_required(VERSION 3.22)
project(opencv_cuda)

set(CMAKE_CXX_STANDARD 17)

# 寻找CUDA
find_package(CUDA REQUIRED)
include_directories(${CUDA_INCLUDE_DIRS})

# 寻找OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(spdlog REQUIRED)

add_executable(opencv_cuda main.cpp )
target_link_libraries(opencv_cuda ${OpenCV_LIBS} ${CUDA_LIBRARIES} spdlog::spdlog systemd ${CUDA_cublas_LIBRARY})
