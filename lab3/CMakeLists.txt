cmake_minimum_required(VERSION 3.31)
project(lab3)

set(CMAKE_CXX_STANDARD 20)

add_executable(sound_processor src/main.cpp
        src/converters-factory/converter_factory.h
        src/converters-factory/converter_factory.cpp

        src/converters/iconverter.h
        src/converters/iconverter.cpp

        src/cfg-parser/config_parser.cpp
        src/cfg-parser/config_parser.h

        src/cfg-parser/token.cpp
        src/cfg-parser/token.h

        src/cfg-parser/lexer.cpp
        src/cfg-parser/lexer.h

        src/cfg-parser/parser.cpp
        src/cfg-parser/parser.h

        src/cfg-parser/point.cpp
        src/cfg-parser/point.h

        src/terminal/color.h
        src/converters/converters.cpp
        src/converters/converters.h)
