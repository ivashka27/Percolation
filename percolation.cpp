#include "percolation.h"

WeightedQuickUnion::WeightedQuickUnion(int n) : parent(n), sz(n, 1), count(n) {
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }
}

int WeightedQuickUnion::find(int p) const {
    while (p != parent[p]) {
        parent[p] = parent[parent[p]];
        p = parent[p];
    }
    return p;
}

void WeightedQuickUnion::connect(int p, int q) {
    int rootP = find(p);
    int rootQ = find(q);
    if (rootP == rootQ) return;

    if (sz[rootP] < sz[rootQ]) {
        parent[rootP] = rootQ;
        sz[rootQ] += sz[rootP];
    }
    else {
        parent[rootQ] = rootP;
        sz[rootP] += sz[rootQ];
    }
    count--;
}

bool WeightedQuickUnion::connected(int p, int q) const {
    return find(p) == find(q);
}

int WeightedQuickUnion::getCount() const {
    return count;
}

Percolation::Percolation(int dimension)
    : dimension(dimension),
    grid(dimension* dimension, false),
    uf(dimension* dimension + 2),
    uf_top(dimension* dimension + 1),
    top_virtual(dimension* dimension),
    bottom_virtual(dimension* dimension + 1),
    num_open_sites(0) {
}

int Percolation::to1D(int row, int col) const {
    return row * dimension + col;
}

void Percolation::open(int row, int col) {
    if (row < 0 || row >= dimension || col < 0 || col >= dimension) {
        throw std::out_of_range("Row or col out of bounds");
    }

    int idx = to1D(row, col);
    if (isOpen(row, col)) return;

    grid[idx] = true;
    num_open_sites++;

    if (row == 0) {
        uf.connect(top_virtual, idx);
        uf_top.connect(top_virtual, idx);
    }

    if (row == dimension - 1) {
        uf.connect(bottom_virtual, idx);
    }

    int neighbors[4][2] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

    for (auto& neighbor : neighbors) {
        int r = row + neighbor[0];
        int c = col + neighbor[1];

        if (r >= 0 && r < dimension && c >= 0 && c < dimension) {
            if (isOpen(r, c)) {
                int n_idx = to1D(r, c);
                uf.connect(idx, n_idx);
                uf_top.connect(idx, n_idx);
            }
        }
    }
}

bool Percolation::isOpen(int row, int col) const {
    if (row < 0 || row >= dimension || col < 0 || col >= dimension) {
        throw std::out_of_range("Row or col out of bounds");
    }
    return grid[to1D(row, col)];
}

bool Percolation::isFull(int row, int col) const {
    if (row < 0 || row >= dimension || col < 0 || col >= dimension) {
        throw std::out_of_range("Row or col out of bounds");
    }
    return uf_top.connected(to1D(row, col), top_virtual);
}

int Percolation::numberOfOpenSites() const {
    return num_open_sites;
}

bool Percolation::percolates() const {
    return uf.connected(top_virtual, bottom_virtual);
}
