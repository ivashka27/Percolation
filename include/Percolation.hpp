#pragma once

#include <vector>
#include <cstddef>

class Percolation {
public:
    Percolation(std::size_t n);
    void open(std::size_t row, std::size_t col);
    bool is_open(std::size_t row, std::size_t col) const;
    bool percolates();
    std::size_t number_of_open_sites() const;

private:
    std::size_t grid_size;
    std::size_t open_count;
    std::vector<bool> opened;
    std::vector<int> parent;
    std::vector<int> size;
    int virtual_top;
    int virtual_bottom;

    int get_index(std::size_t row, std::size_t col) const;
    int find(int i);
    void unite(int p, int q);
};