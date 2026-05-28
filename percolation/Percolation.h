#pragma once

#include "DisjointSet.h"

#include <cstddef>
#include <vector>

class Percolation {
public:
    explicit Percolation(size_t dimension);

    void open(size_t row, size_t col);
    bool is_open(size_t row, size_t col) const;
    bool is_full(size_t row, size_t col) const;
    bool percolates() const;

    size_t get_dimension() const;
    size_t get_open_sites_count() const;

private:
    size_t index(size_t row, size_t col) const;
    void validate(size_t row, size_t col) const;

    size_t dimension_;
    size_t virtual_top_;
    size_t virtual_bottom_;
    size_t open_sites_count_;
    std::vector<bool> open_sites_;
    DisjointSet flow_;
    DisjointSet fullness_;
};
