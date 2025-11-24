project(Google_test)

find_package(GTest REQUIRED)


include_directories(${gtest_SORUCE_DIR}/include
        ${gtest_SOURCE_DIR})
include_directories(../src)

add_executable(gtest_run boat.cpp
        ../src/core/Core.h
        ../src/core/CellPos.cpp
        ../src/core/Universe.cpp
        ../src/core/Rules.cpp
        ../src/core/Field.cpp
        ../src/file-handler/file_handler.cpp
        rules.cpp
        universe.cpp
        save_load.cpp)

target_link_libraries(gtest_run GTest::gtest GTest::gtest_main)

# Убедитесь, что включаемые пути правильные
target_include_directories(gtest_run PRIVATE src)