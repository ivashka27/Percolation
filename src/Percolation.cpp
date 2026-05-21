#include "Percolation.h"
#include <queue>

Percolation::Percolation(size_t n): 
    size(n), 
    grid(n * n, false), 
    open_sites(0) {}

void Percolation::open(size_t row, size_t col) {
    if (is_open(row, col)) return;
    grid[row * size + col] = true;
    open_sites++;
}

bool Percolation::is_open(size_t row, size_t col) const {
    return grid[row * size + col];
}

bool Percolation::percolates() const {
    if (size == 0) return false;

    std::queue<std::pair<size_t, size_t>> q;
    std::vector<bool> visited(size * size, false);

    for (size_t col = 0; col < size; ++col) {
        if (is_open(0, col)) {
            q.push({0, col});
            visited[col] = true;
        }
    }

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!q.empty()) {
        auto [r, c] = q.front();
        q.pop();

        if (r == size - 1) {
            return true;
        }

        for (int i = 0; i < 4; ++i) {
            int nr = static_cast<int>(r) + dr[i];
            int nc = static_cast<int>(c) + dc[i];

            if (nr >= 0 && nr < static_cast<int>(size) && nc >= 0 && nc < static_cast<int>(size)) {
                size_t next_row = static_cast<size_t>(nr);
                size_t next_col = static_cast<size_t>(nc);
                size_t next_idx = next_row * size + next_col;

                if (is_open(next_row, next_col) && !visited[next_idx]) {
                    visited[next_idx] = true;
                    q.push({next_row, next_col});
                }
            }
        }
    }

    return false;
}

size_t Percolation::get_open_sites_count() const {
    return open_sites;
}
