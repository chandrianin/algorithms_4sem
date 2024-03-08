cmake_minimum_required(VERSION 3.27)
project(02_03_lab)

set(CMAKE_CXX_STANDARD 17)

include_directories(inc)
include_directories(googletest/googlemock/include)
include_directories(googletest/googletest/include)

enable_testing()
add_subdirectory(googletest)

add_executable(02_03_lab
        inc/func.h
        main.cpp)

target_link_libraries(02_03_lab gtest_main gmock_main)

FILE(COPY "data/input.txt" DESTINATION "data/")