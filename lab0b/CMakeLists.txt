cmake_minimum_required(VERSION 3.31)
project(lab0b)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab0b main.cpp
                    WordCounter.cpp
                    WordCounter.h
                    WordsBuffer.cpp
                    WordsBuffer.h
                    Saver.cpp
                    Saver.h)
