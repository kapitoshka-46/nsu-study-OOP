cmake_minimum_required(VERSION 3.31)
project(lab1a)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab1a
        src/main.cpp
        src/BitArray.h
        src/BitArray.cpp
        )

add_subdirectory(gtest)