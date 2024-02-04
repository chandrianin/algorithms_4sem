cmake_minimum_required(VERSION 3.27)
project(02_lab)

set(CMAKE_CXX_STANDARD 17)

include_directories(inc)

add_executable(02_lab
        inc/func.h
        main.cpp)

FILE(COPY "data/input.txt" DESTINATION "data/")