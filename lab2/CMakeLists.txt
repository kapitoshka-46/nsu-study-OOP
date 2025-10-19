cmake_minimum_required(VERSION 3.31)
project(game-of-life)

set(CMAKE_CXX_STANDARD 20)

add_executable(game-of-life main.cpp
        TerminalEmulator.cpp
        TerminalEmulator.h
        Command.cpp
        Command.h)
