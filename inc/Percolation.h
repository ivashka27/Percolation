#pragma once
#include <vector>
#include <cstddef>
#include "../inc/UnionFind.h"

class Percolation 
{
private:
    size_t n;
    std::vector<bool> opened;
    UnionFind uf;

    size_t top;
    size_t bottom;
    size_t openedCount;

    size_t index(size_t row, size_t col) const;
    bool is_valid(size_t row, size_t col) const;

public:
    explicit Percolation(size_t dimension);

    void open(size_t row, size_t col);
    bool is_open(size_t row, size_t col) const;
    bool is_full(size_t row, size_t col);
    bool percolates();

    size_t get_opened_count() const;
    size_t get_size() const;
};