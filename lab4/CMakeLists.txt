cmake_minimum_required(VERSION 3.31)
project(csv-parser)

set(CMAKE_CXX_STANDARD 26)

add_executable(csv-parser main.cpp
        tuple_extension.h
        csv_parser.cpp
        csv_parser.h)
