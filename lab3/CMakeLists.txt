cmake_minimum_required(VERSION 3.31)
project(lab3)

set(CMAKE_CXX_STANDARD 20)

add_executable(sound_processor src/main.cpp
    src/converters-factory/converter_factory.h src/converters-factory/converter_factory.cpp
    src/converters/iconverter.h src/converters/iconverter.cpp)
