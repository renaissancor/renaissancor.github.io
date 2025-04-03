cmake_minimum_required(VERSION 3.29)
project(h4 C)

set(CMAKE_C_STANDARD 11)

include_directories(h4)

add_executable(h4
        code.c
)
