cmake_minimum_required(VERSION 3.10)

project(percolation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(percolation
    src/main.cpp
    src/percolation.cpp
    src/percolation_stats.cpp
)

target_include_directories(percolation PRIVATE include)
