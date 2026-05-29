#pragma once
#include "DisjointSetUnion.h"
#include <vector>

class Percolation {
private:
    size_t size;
    std::vector<bool> opened;
    DisjointSetUnion dsu;
    size_t virtual_top;
    size_t virtual_bottom;
    size_t open_sites_count;

    size_t get_index(size_t row, size_t col) const {
        return (row * size) + col + 1;
    }

public:
    Percolation(size_t n) 
        : size(n), 
          opened(n * n, false), 
          dsu(n * n + 2), 
          virtual_top(0), 
          virtual_bottom(n * n + 1), 
          open_sites_count(0) {}

    void open(size_t row, size_t col) {
        size_t idx = row * size + col;
        if (opened[idx]) return;

        opened[idx] = true;
        open_sites_count++;

        size_t dsu_idx = get_index(row, col);

        if (row == 0) {
            dsu.unite(dsu_idx, virtual_top);
        }
        if (row == size - 1) {
            dsu.unite(dsu_idx, virtual_bottom);
        }

        if (row > 0 && is_open(row - 1, col))    dsu.unite(dsu_idx, get_index(row - 1, col));
        if (row < size - 1 && is_open(row + 1, col)) dsu.unite(dsu_idx, get_index(row + 1, col));
        if (col > 0 && is_open(row, col - 1))    dsu.unite(dsu_idx, get_index(row, col - 1));
        if (col < size - 1 && is_open(row, col + 1)) dsu.unite(dsu_idx, get_index(row, col + 1));
    }

    bool is_open(size_t row, size_t col) const {
        return opened[row * size + col];
    }

    size_t get_open_sites_count() const {
        return open_sites_count;
    }

    bool percolates() {
        return dsu.connected(virtual_top, virtual_bottom);
    }
};