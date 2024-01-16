cmake_minimum_required(VERSION 3.27)
project(01_lab)

set(CMAKE_CXX_STANDARD 14)

include_directories(inc/)

add_executable(01_lab
        main.cpp
        inc/func.h)
