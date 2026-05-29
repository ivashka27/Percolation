cmake_minimum_required(VERSION 3.10)

project(Percolation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Percolation
    main.cpp
    percolation.cpp
    percolation_stats.cpp
)