cmake_minimum_required(VERSION 3.21...3.31)

project(
        physarum
        VERSION 1.0
        DESCRIPTION "OpenGL GPU-accelerated computer simulation of P. polycephalum (AKA slime mold)."
        LANGUAGES C
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS ON)

add_subdirectory(src bin)
