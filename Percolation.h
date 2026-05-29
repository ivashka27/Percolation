#pragma once

#include <vector>
#include <cstddef>
#include "DisjointSet.h"

class Percolation
{
private:
    size_t dimension;
    std::vector<char> opened;
    size_t opened_count;

    DisjointSet union_find;
    DisjointSet union_find_for_fullness;

    size_t virtual_top;
    size_t virtual_bottom;

    size_t get_index(size_t row, size_t col) const;
    void validate(size_t row, size_t col) const;

public:
    explicit Percolation(size_t n);

    void open(size_t row, size_t col);

    bool is_open(size_t row, size_t col) const;

    bool is_full(size_t row, size_t col);

    bool percolates();

    size_t get_open_count() const;

    size_t get_dimension() const;
};