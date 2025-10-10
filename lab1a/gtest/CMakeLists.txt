project(Google_test)

find_package(GTest REQUIRED)


include_directories(${gtest_SORUCE_DIR}/include
                   ${gtest_SOURCE_DIR})
include_directories(../src)

add_executable(gtest_run test_assertion.cpp
                ../src/BitArray.cpp)

target_link_libraries(gtest_run GTest::gtest GTest::gtest_main)

# Убедитесь, что включаемые пути правильные
target_include_directories(gtest_run PRIVATE src)