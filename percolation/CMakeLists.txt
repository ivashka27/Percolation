cmake_minimum_required(VERSION 3.16)
project(percolation_solution LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(percolation
    main.cpp
    Percolation.cpp
    PercolationStats.cpp
)
