#include "Percolation.hpp"
#include <stdexcept>

Percolation::Percolation(size_t dimension) 
    : n(dimension), 
      open_sites(dimension * dimension, false),
      parent(dimension * dimension + 2),
      sz(dimension * dimension + 2, 1),
      percolates_flag(false) {
    
    BOTTOM_VIRTUAL = n * n + 1;
    
    for (size_t i = 0; i <= n * n + 1; ++i) {
        parent[i] = i;
    }
}

void Percolation::open(size_t row, size_t col) {
    if (row >= n || col >= n) {
        throw std::out_of_range("Row or column out of range");
    }
    if (!is_open(row, col)) {
        open_sites[row * n + col] = true;
        connect_neighbors(row, col);
    }
}

bool Percolation::is_open(size_t row, size_t col) const {
    if (row >= n || col >= n) {
        throw std::out_of_range("Row or column out of range");
    }
    return open_sites[row * n + col];
}

bool Percolation::percolates() const {
    return percolates_flag;
}

size_t Percolation::get_number_of_open_sites() const {
    size_t count = 0;
    for (bool site : open_sites) {
        if (site) count++;
    }
    return count;
}