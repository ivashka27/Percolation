#pragma once
#include <vector>
#include <stdexcept>
#include <numeric>

class UnionFind {
    std::vector<int> parent_;
    std::vector<int> rank_;
public:
    explicit UnionFind(int n) : parent_(n), rank_(n, 0) {
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    int find(int x) {
        if (parent_[x] != x) parent_[x] = find(parent_[x]);
        return parent_[x];
    }

    int find_const(int x) const {
        while (parent_[x] != x) x = parent_[x];
        return x;
    }

    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;
        if (rank_[px] < rank_[py]) std::swap(px, py);
        parent_[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
    }

    bool connected(int x, int y) {
        return find(x) == find(y);
    }

    bool connected(int x, int y) const {
        return find_const(x) == find_const(y);
    }
};

class Percolation {
    size_t n_;
    std::vector<bool> open_;
    UnionFind uf_;
    int virtual_top_;
    int virtual_bottom_;
    size_t open_count_;

    int idx(int row, int col) const {
        return static_cast<int>(row * n_ + col);
    }

public:
    explicit Percolation(size_t n) : n_(n), open_(n * n, false), uf_(static_cast<int>(n * n + 2)), virtual_top_(static_cast<int>(n * n)), virtual_bottom_(static_cast<int>(n * n + 1)), open_count_(0) {
        if (n == 0) throw std::invalid_argument("Grid size must be > 0");
    }

    void open_cell(size_t row, size_t col) {
        if (row >= n_ || col >= n_) throw std::out_of_range("Cell index out of range");
        if (open_[idx(row, col)]) return;

        open_[idx(row, col)] = true;
        open_count_++;

        int cell = idx(row, col);

        if (row == 0) uf_.unite(cell, virtual_top_);
        if (row == n_ - 1) uf_.unite(cell, virtual_bottom_);

        const int dr[] = {-1, 1, 0, 0};
        const int dc[] = {0, 0, -1, 1};
        for (int d = 0; d < 4; ++d) {
            int nr = static_cast<int>(row) + dr[d];
            int nc = static_cast<int>(col) + dc[d];
            if (nr >= 0 && nr < static_cast<int>(n_) && nc >= 0 && nc < static_cast<int>(n_) && open_[idx(nr, nc)]) {
                uf_.unite(cell, idx(nr, nc));
            }
        }
    }

    bool is_open(size_t row, size_t col) const {
        return open_[idx(row, col)];
    }

    bool is_full(size_t row, size_t col) const {
        return open_[idx(row, col)] && uf_.connected(idx(row, col), virtual_top_);
    }

    bool percolates() const {
        return uf_.connected(virtual_top_, virtual_bottom_);
    }

    size_t number_of_open_sites() const {
        return open_count_;
    }

    size_t size() const {
        return n_;
    }
};