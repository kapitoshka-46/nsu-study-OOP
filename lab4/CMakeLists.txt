cmake_minimum_required(VERSION 3.31)
project(csv-parser)

set(CMAKE_CXX_STANDARD 26)

find_package(spdlog REQUIRED)
find_package(fmt REQUIRED)

add_executable(csv-parser main.cpp
        tuple_extension.h
        csv_parser.cpp
        csv_parser.h)

target_link_libraries(csv-parser PRIVATE spdlog::spdlog fmt::fmt)
