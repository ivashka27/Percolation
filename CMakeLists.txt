cmake_minimum_required(VERSION 3.16)
project(Percolation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(percolation
    percolation.cpp
    percolation_stats.cpp
)

target_include_directories(percolation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(percolation_tests
    test.cpp
)

target_link_libraries(percolation_tests PRIVATE percolation)

enable_testing()
add_test(NAME percolation_tests COMMAND percolation_tests)
