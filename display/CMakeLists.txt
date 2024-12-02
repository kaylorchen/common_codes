cmake_minimum_required(VERSION 3.21)
project(display)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE ${OpenCV_LIBS} X11)
target_include_directories(main PRIVATE ${OpenCV_INCLUDE_DIRS})

add_executable(main2 main2.cpp)
target_link_libraries(main2 PRIVATE ${OpenCV_LIBS})
target_include_directories(main2 PRIVATE ${OpenCV_INCLUDE_DIRS})
