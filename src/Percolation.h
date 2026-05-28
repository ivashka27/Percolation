#pragma once

#include <vector>
#include <cstddef>

class Percolation
{
public:
    Percolation(size_t n);

    void open(size_t row, size_t col);

    bool is_open(size_t row, size_t col) const;

    bool percolates() const;

    size_t number_of_open_sites() const;

private:
    size_t n;
    std::vector<std::vector<bool>> grid;

    bool dfs(size_t row, size_t col, std::vector<std::vector<bool>>& used) const;
};