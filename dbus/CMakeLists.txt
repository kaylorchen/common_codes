cmake_minimum_required(VERSION 3.30)
project(systemd_manager)

set(CMAKE_CXX_STANDARD 20)

find_package(kaylordut REQUIRED)

add_executable(${PROJECT_NAME} main.cpp
        systemd_manager.cpp)
target_link_libraries(${PROJECT_NAME} ${kaylordut_LIBS})
