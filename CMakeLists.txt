cmake_minimum_required(VERSION 3.0)
project(daniyar)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

set(SOURCE_FILES main.cpp Helper.h vehicle.txt driver.txt)
add_executable(daniyar ${SOURCE_FILES})