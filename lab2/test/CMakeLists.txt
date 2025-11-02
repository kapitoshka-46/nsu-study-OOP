project(Google_test)

find_package(GTest REQUIRED)


include_directories(${gtest_SORUCE_DIR}/include
        ${gtest_SOURCE_DIR})
include_directories(../src)

add_executable(gtest_run test0.cpp
        ../src/Core.cpp
        ../src/Core.h
        Rules.cpp)

target_link_libraries(gtest_run GTest::gtest GTest::gtest_main)

# Убедитесь, что включаемые пути правильные
target_include_directories(gtest_run PRIVATE src)