cmake_minimum_required(VERSION 3.31)
project(life)

set(CMAKE_CXX_STANDARD 20)

add_executable(life src/main.cpp
        src/core/Core.h
        src/core/Universe.cpp
        src/core/CellPos.cpp
        src/file-handler/file_handler.cpp
        src/file-handler/file_handler.h
        src/core/Rules.cpp
        src/core/Field.cpp
        src/Terminal/Terminal.cpp
        src/Terminal/Terminal.h
        src/game-controller/GameController.cpp
        src/game-controller/GameController.h
        src/signal-handler/signal_handler.cpp
        src/signal-handler/signal_handler.h
)

add_subdirectory(test)