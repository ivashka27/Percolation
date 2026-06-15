#include "Percolation.hpp"

Percolation::Percolation(std::size_t n) : grid_size(n), open_count(0) {
    std::size_t total_sites = n * n + 2;
    opened.assign(n * n, false);
    parent.resize(total_sites);
    size.assign(total_sites, 1);

    virtual_top = n * n;
    virtual_bottom = n * n + 1;

    for (std::size_t i = 0; i < total_sites; ++i) {
        parent[i] = i;
    }
}

int Percolation::get_index(std::size_t row, std::size_t col) const {
    return row * grid_size + col;
}

int Percolation::find(int i) {
    int root = i;
    while (root != parent[root]) {
        root = parent[root];
    }
    int curr = i;
    while (curr != root) {
        int nxt = parent[curr];
        parent[curr] = root;
        curr = nxt;
    }
    return root;
}

void Percolation::unite(int p, int q) {
    int rootP = find(p);
    int rootQ = find(q);
    if (rootP == rootQ) return;

    if (size[rootP] < size[rootQ]) {
        parent[rootP] = rootQ;
        size[rootQ] += size[rootP];
    } else {
        parent[rootQ] = rootP;
        size[rootP] += size[rootQ];
    }
}

void Percolation::open(std::size_t row, std::size_t col) {
    int idx = get_index(row, col);
    if (opened[idx]) return;

    opened[idx] = true;
    open_count++;

    if (row == 0) {
        unite(idx, virtual_top);
    }
    if (row == grid_size - 1) {
        unite(idx, virtual_bottom);
    }

    if (row > 0 && is_open(row - 1, col)) {
        unite(idx, get_index(row - 1, col));
    }
    if (row < grid_size - 1 && is_open(row + 1, col)) {
        unite(idx, get_index(row + 1, col));
    }
    if (col > 0 && is_open(row, col - 1)) {
        unite(idx, get_index(row, col - 1));
    }
    if (col < grid_size - 1 && is_open(row, col + 1)) {
        unite(idx, get_index(row, col + 1));
    }
}

bool Percolation::is_open(std::size_t row, std::size_t col) const {
    return opened[get_index(row, col)];
}

bool Percolation::percolates() {
    if (grid_size == 0) return false;
    return find(virtual_top) == find(virtual_bottom);
}

std::size_t Percolation::number_of_open_sites() const {
    return open_count;
}