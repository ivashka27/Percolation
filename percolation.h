#pragma once

#include <vector>
#include <stdexcept>

/**
 * Percolation model based on an n×n grid.
 * Uses Union-Find (Weighted Quick Union with path compression).
 *
 * Two virtual nodes are used:
 *   - virtual_top    (index n*n)   — connected to all open cells in row 0
 *   - virtual_bottom (index n*n+1) — connected to all open cells in row n-1
 *
 * The system percolates when virtual_top and virtual_bottom are in the same component.
 */
class Percolation {
public:
    /**
     * Creates an n×n grid with all cells blocked.
     * @throws std::invalid_argument if n == 0
     */
    explicit Percolation(size_t n)
        : n_(n),
          open_(n * n, false),
          open_count_(0),
          parent_(n * n + 2),
          rank_(n * n + 2, 0),
          virtual_top_(n * n),
          virtual_bottom_(n * n + 1)
    {
        if (n == 0) throw std::invalid_argument("Grid size must be > 0");
        // Each node is its own parent initially
        for (size_t i = 0; i < parent_.size(); ++i) parent_[i] = i;
    }

    /**
     * Opens cell (row, col) if not already open. 0-indexed.
     * @throws std::out_of_range if coordinates are invalid
     */
    void open(size_t row, size_t col) {
        validate(row, col);
        size_t idx = index(row, col);
        if (open_[idx]) return;

        open_[idx] = true;
        ++open_count_;

        // Connect to virtual top / bottom
        if (row == 0)     unite(idx, virtual_top_);
        if (row == n_ - 1) unite(idx, virtual_bottom_);

        // Connect to open neighbours (up, down, left, right)
        const int dr[] = {-1, 1, 0, 0};
        const int dc[] = {0, 0, -1, 1};
        for (int d = 0; d < 4; ++d) {
            int nr = static_cast<int>(row) + dr[d];
            int nc = static_cast<int>(col) + dc[d];
            if (nr >= 0 && nr < static_cast<int>(n_) &&
                nc >= 0 && nc < static_cast<int>(n_) &&
                open_[index(nr, nc)])
            {
                unite(idx, index(nr, nc));
            }
        }
    }

    /** Returns true if cell (row, col) is open. */
    bool is_open(size_t row, size_t col) const {
        validate(row, col);
        return open_[index(row, col)];
    }

    /**
     * Returns true if cell (row, col) is "full"
     * (connected to the top row through open cells).
     */
    bool is_full(size_t row, size_t col) const {
        validate(row, col);
        return open_[index(row, col)] && find(index(row, col)) == find(virtual_top_);
    }

    /** Returns the number of open cells. */
    size_t number_of_open_sites() const { return open_count_; }

    /** Returns true if the system percolates. */
    bool percolates() const {
        return find(virtual_top_) == find(virtual_bottom_);
    }

    /** Returns grid dimension. */
    size_t size() const { return n_; }

private:
    size_t n_;
    std::vector<bool>   open_;
    size_t              open_count_;
    mutable std::vector<size_t> parent_;
    std::vector<size_t> rank_;
    size_t virtual_top_;
    size_t virtual_bottom_;

    size_t index(size_t row, size_t col) const { return row * n_ + col; }
    size_t index(int row, int col) const {
        return static_cast<size_t>(row) * n_ + static_cast<size_t>(col);
    }

    void validate(size_t row, size_t col) const {
        if (row >= n_ || col >= n_)
            throw std::out_of_range("Cell coordinates out of range");
    }

    // Path-compression find
    size_t find(size_t x) const {
        while (parent_[x] != x) {
            parent_[x] = parent_[parent_[x]]; // path halving
            x = parent_[x];
        }
        return x;
    }

    // Union by rank
    void unite(size_t a, size_t b) {
        size_t ra = find(a), rb = find(b);
        if (ra == rb) return;
        if (rank_[ra] < rank_[rb]) std::swap(ra, rb);
        parent_[rb] = ra;
        if (rank_[ra] == rank_[rb]) ++rank_[ra];
    }
};
