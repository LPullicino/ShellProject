cmake_minimum_required(VERSION 3.16)
project(ShellProject)

set(CMAKE_CXX_STANDARD 14)

add_executable(ShellProject main.cpp linenoise.h linenoise.c functions.cpp functions.h)