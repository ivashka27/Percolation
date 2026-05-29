#include "percolation.h"
#include <utility>

Percolation::DSU::DSU(size_t n) {
    parent.resize(n);
    sz.resize(n, 1);
    for (size_t i = 0; i < n; ++i) {
        parent[i] = i;
    }
}

size_t Percolation::DSU::find(size_t v) {
    if (parent[v] == v) {
        return v;
    }
    parent[v] = find(parent[v]);
    return parent[v];
}

void Percolation::DSU::unite(size_t a, size_t b) {
    a = find(a);
    b = find(b);
    if (a == b) {
        return;
    }
    if (sz[a] < sz[b]) {
        std::swap(a, b);
    }
    parent[b] = a;
    sz[a] += sz[b];
}

bool Percolation::DSU::connected(size_t a, size_t b) {
    return find(a) == find(b);
}

Percolation::Percolation(size_t dimension)
    : n(dimension),
      opened(dimension * dimension, false),
      dsu(dimension * dimension + 2),
      virtual_top(dimension * dimension),
      virtual_bottom(dimension * dimension + 1),
      opened_count(0) {
}

size_t Percolation::index(size_t row, size_t col) const {
    return row * n + col;
}

bool Percolation::valid(size_t row, size_t col) const {
    return row < n && col < n;
}

void Percolation::open(size_t row, size_t col) {
    if (!valid(row, col)) {
        return;
    }
    size_t current = index(row, col);
    if (opened[current]) {
        return;
    }

    opened[current] = true;
    ++opened_count;

    if (row == 0) {
        dsu.unite(current, virtual_top);
    }
    if (row == n - 1) {
        dsu.unite(current, virtual_bottom);
    }
    if (row > 0 && is_open(row - 1, col)) {
        dsu.unite(current, index(row - 1, col));
    }
    if (row + 1 < n && is_open(row + 1, col)) {
        dsu.unite(current, index(row + 1, col));
    }
    if (col > 0 && is_open(row, col - 1)) {
        dsu.unite(current, index(row, col - 1));
    }
    if (col + 1 < n && is_open(row, col + 1)) {
        dsu.unite(current, index(row, col + 1));
    }
}

bool Percolation::is_open(size_t row, size_t col) const {
    if (!valid(row, col)) {
        return false;
    }
    return opened[index(row, col)];
}

bool Percolation::percolates() {
    return dsu.connected(virtual_top, virtual_bottom);
}

size_t Percolation::get_opened_count() const {
    return opened_count;
}