cmake_minimum_required(VERSION 3.31)
project(lab3)

set(CMAKE_CXX_STANDARD 20)

add_executable(sound_processor src/main.cpp

        src/cfg-parser/config_parser.cpp
        src/cfg-parser/config_parser.h

        src/cfg-parser/token.cpp
        src/cfg-parser/token.h

        src/cfg-parser/lexer.cpp
        src/cfg-parser/lexer.h

        src/cfg-parser/point.h

        src/terminal/color.h
        src/converters/i_converter.cpp
        src/converters/i_converter.h

        src/converters/mix/mix.cpp
        src/converters/mix/mix.h

        src/converters/mute/mute.cpp
        src/converters/mute/mute.h

        src/converters-factory/converter_factory.cpp
        src/converters-factory/converter_factory.h
        src/sample-stream/ISampleStream.h
        src/sample-stream/wav/wav_stream.cpp
        src/sample-stream/wav/wav_stream.h
        src/sound-processor/sound_processor.cpp
        src/sound-processor/sound_processor.h
        src/converters/dummy/dummy.cpp
        src/converters/dummy/dummy.h
)
