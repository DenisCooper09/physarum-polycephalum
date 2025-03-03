cmake_minimum_required(VERSION 3.21...3.31)
project(glad LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS ON)

add_subdirectory(src)

target_include_directories(glad PRIVATE include)
