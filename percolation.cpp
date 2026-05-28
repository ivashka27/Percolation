#include "Percolation.h"
#include <stdexcept>

// --- UnionFind ---

Percolation::UnionFind::UnionFind(size_t n) 
    : parent(n), rank(n, 0) 
{
    for (size_t i = 0; i < n; ++i) {
        parent[i] = i;
    }
}

size_t Percolation::UnionFind::find(size_t x) {
    if (parent[x] != x) {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

void Percolation::UnionFind::union_sets(size_t x, size_t y) {
    size_t root_x = find(x);
    size_t root_y = find(y);
    
    if (root_x == root_y) return;

    if (rank[root_x] < rank[root_y]) {
        parent[root_x] = root_y;
    } else if (rank[root_x] > rank[root_y]) {
        parent[root_y] = root_x;
    } else {
        parent[root_y] = root_x;
        ++rank[root_x];
    }
}

// --- Percolation ---

Percolation::Percolation(size_t n)
    : dimension(n)
    , open_grid(n * n, false)
    , open_count(0)
    , uf(n * n + 2)
    , top_virtual(n * n)
    , bottom_virtual(n * n + 1)
{
    for (size_t col = 0; col < dimension; ++col) {
        uf.union_sets(top_virtual, index(0, col));
        uf.union_sets(bottom_virtual, index(dimension - 1, col));
    }
}

void Percolation::open(size_t row, size_t col) {
    validate(row, col);
    size_t idx = index(row, col);
    
    if (open_grid[idx]) return;

    open_grid[idx] = true;
    ++open_count;

    if (row > 0 && open_grid[index(row - 1, col)]) {
        uf.union_sets(idx, index(row - 1, col));
    }
    if (row < dimension - 1 && open_grid[index(row + 1, col)]) {
        uf.union_sets(idx, index(row + 1, col));
    }
    if (col > 0 && open_grid[index(row, col - 1)]) {
        uf.union_sets(idx, index(row, col - 1));
    }
    if (col < dimension - 1 && open_grid[index(row, col + 1)]) {
        uf.union_sets(idx, index(row, col + 1));
    }
}

bool Percolation::is_open(size_t row, size_t col) const {
    validate(row, col);
    return open_grid[index(row, col)];
}

bool Percolation::has_percolation() {
    return uf.find(top_virtual) == uf.find(bottom_virtual);
}

size_t Percolation::number_of_open_cells() const {
    return open_count;
}

size_t Percolation::index(size_t row, size_t col) const {
    return row * dimension + col;
}

void Percolation::validate(size_t row, size_t col) const {
    if (row >= dimension || col >= dimension) {
        throw std::out_of_range("Index out of bounds");
    }
}
