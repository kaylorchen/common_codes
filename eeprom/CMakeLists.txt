cmake_minimum_required(VERSION 3.21)
project(eeprom)

set(CMAKE_CXX_STANDARD 17)
# 寻找Boost库
find_package(Boost REQUIRED COMPONENTS system)

# 包含 directories
include_directories(${Boost_INCLUDE_DIRS})
find_package(spdlog REQUIRED)
add_executable(eeprom main.cpp eeprom.cpp)
target_link_libraries(eeprom spdlog::spdlog systemd ${Boost_LIBRARIES})
