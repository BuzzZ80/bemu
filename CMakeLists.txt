cmake_minimum_required(VERSION 3.12)

project(emulator)
include_directories(include)
add_executable(emulator main.cpp cpuCore.cpp)
