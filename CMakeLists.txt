cmake_minimum_required(VERSION 3.26)
project(hex)

set(CMAKE_CXX_STANDARD 17)

add_executable(hex main.cpp
        include/board.h
        src/board.cpp)
