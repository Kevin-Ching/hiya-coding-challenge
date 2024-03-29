cmake_minimum_required(VERSION 3.13)

# Set C++ standard to C++11
set(CMAKE_CXX_STANDARD 11)

project(short_callers)

add_executable(short_callers)
target_sources(short_callers PRIVATE 
    src/short_callers.cpp
    src/short_callers.h
)