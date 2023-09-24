cmake_minimum_required(VERSION 3.15)
project(labwork_10_LomanTrue2)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_subdirectory(cpr)
add_subdirectory(json)
add_subdirectory(lib)
add_subdirectory(bin)

include_directories(${CPR_INCLUDE_DIRS})
