cmake_minimum_required(VERSION 3.31)
project(lab2)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab2 src/main.cpp
        src/Core.h
        src/Universe.cpp
        src/CellPos.cpp
        src/CellPos.h
        src/file_handler.cpp
        src/file_handler.h
        src/Rules.cpp
        src/Field.cpp
)

add_subdirectory(test)