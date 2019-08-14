cmake_minimum_required(VERSION 3.1)
project(RingBuffer)
add_compile_options(-std=c++11) # CMake 2.8.12 or newer

add_library(${PROJECT_NAME} INTERFACE)
target_sources(${PROJECT_NAME} INTERFACE Ringbuffer.h)
target_include_directories(${PROJECT_NAME} INTERFACE "${PROJECT_SOURCE_DIR}/RingBuffer")

add_executable(example example.cpp)