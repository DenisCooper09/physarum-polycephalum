cmake_minimum_required(VERSION 3.21...3.31)

project(
        physarum
        VERSION 1.0
        DESCRIPTION "OpenGL GPU-accelerated computer simulation of P. polycephalum (AKA slime mold)."
        LANGUAGES C
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS ON)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(PP_DEBUG_BUILD)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(PP_RELEASE_BUILD)
endif ()

add_subdirectory(src bin)

target_include_directories(physarum PRIVATE include)

include(cmake/Libraries.cmake)

list(APPEND LIBRARIES glad glfw)

build_libraries("${LIBRARIES}")
link_libraries(physarum "${LIBRARIES}")
