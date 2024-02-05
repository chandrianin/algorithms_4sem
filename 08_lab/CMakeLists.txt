cmake_minimum_required(VERSION 3.27)
project(08_lab)

set(CMAKE_CXX_STANDARD 17)

include_directories(inc)

add_executable(08_lab
        inc/func.h
        main.cpp)

FILE(COPY "data/input.txt" DESTINATION "data/")