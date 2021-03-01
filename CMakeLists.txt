cmake_minimum_required(VERSION 3.12)

project(bemu)
include_directories(src)
add_executable(bemu src/main.cpp src/cpuCore.cpp)
file(COPY src/help.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
