cmake_minimum_required(VERSION 3.15)
project(serial_read)

set(CMAKE_CXX_STANDARD 20)

add_executable(serial_read cmake-build-debug/main.cpp cmake-build-debug/Port.cpp cmake-build-debug/Port.h)