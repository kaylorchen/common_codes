cmake_minimum_required(VERSION 3.30)
project(systemd_manager)

set(CMAKE_CXX_STANDARD 20)

find_package(kaylordut REQUIRED)
find_package(DBus1 REQUIRED)
include_directories(${DBus1_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} main.cpp
        systemd_manager.cpp)
target_link_libraries(${PROJECT_NAME} ${kaylordut_LIBS} ${DBus1_LIBRARIES})
