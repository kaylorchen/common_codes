cmake_minimum_required(VERSION 3.22)
set(CMAKE_CUDA_COMPILER /usr/local/cuda/bin/nvcc)
set(CUDA_TOOLKIT_ROOT_DIR "/usr/local/cuda")
project(matmul LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3")

find_package(kaylordut REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
find_package(CUDA REQUIRED)
include_directories(${CUDA_INCLUDE_DIRS})
set(CUDA_LIBS ${CUDA_LIBRARIES} ${CUDA_cublas_LIBRARY})

add_executable(matmul
        matmul.cpp)
target_link_libraries(matmul ${kaylordut_LIBS} ${OpenCV_LIBS} ${CUDA_LIBS})
