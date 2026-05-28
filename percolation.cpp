#include "Percolation.h"
#include <queue>

Percolation::Percolation(size_t n) : n_(n), grid_(n, std::vector<bool>(n, false)), full_(n, std::vector<bool>(n, false)) {}
void Percolation::open(size_t row, size_t col) {
    if (!grid_[row][col]) {
        grid_[row][col] = true;
        fill(row, col);
    }
}
bool Percolation::isOpen(size_t row, size_t col) const {
    return grid_[row][col];
}
bool Percolation::isFull(size_t row, size_t col) const {
    return full_[row][col];
}
void Percolation::fill(size_t row, size_t col) {
    if (!grid_[row][col] || full_[row][col]) return;
    std::queue<std::pair<size_t, size_t>> q;
    q.push({row, col});
    full_[row][col] = true;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    while (!q.empty()) {
        auto [r, c] = q.front(); q.pop();
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            if (nr >= 0 && nr < n_ && nc >= 0 && nc < n_ && grid_[nr][nc] && !full_[nr][nc]) {
                full_[nr][nc] = true;
                q.push({nr, nc});
            }
        }
    }
}
bool Percolation::percolates() const {
    for (size_t col = 0; col < n_; ++col) {
        if (full_[n_-1][col]) return true;
    }
    return false;
}

size_t Percolation::numberOfOpenSites() const {
    size_t count = 0;
    for (auto &row : grid_) {
        for (bool cell : row) {
            if (cell) ++count;
        }
    }
    return count;
}