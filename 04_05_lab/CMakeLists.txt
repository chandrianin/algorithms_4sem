cmake_minimum_required(VERSION 3.27)
project(04_05_lab)

set(CMAKE_CXX_STANDARD 17)

include_directories(inc)
include_directories(googletest/googletest/include)
#include_directories(googletest/googlemock/include)

enable_testing()
add_subdirectory(googletest)

add_executable(04_05_lab
        inc/func.h
        tests/test.h
        main.cpp)

target_link_libraries(04_05_lab gtest_main gmock_main)

FILE(COPY "data/input.txt" DESTINATION "data/")

FILE(COPY "tests/testInput1.txt" DESTINATION "tests")
FILE(COPY "tests/testInput2.txt" DESTINATION "tests")