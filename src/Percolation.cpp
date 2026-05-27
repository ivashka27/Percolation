#include "Percolation.hpp"
#include <stdexcept>

int Percolation::get_index(std::size_t row, std::size_t col) const {
    return static_cast<int>(row * n + col);
}

void Percolation::check_bounds(std::size_t row, std::size_t col) const {
    if (row >= n || col >= n) {
        throw std::out_of_range("Индекс выходит за границы решетки");
    }
}

Percolation::Percolation(std::size_t dimension) :
    n(dimension),
    opened(n * n, false),
    open_sites_count(0),
    dsu_perc(static_cast<int>(n * n + 2)),
    dsu_full(static_cast<int>(n * n + 1)),
    virtual_top(static_cast<int>(n * n)),
    virtual_bottom(static_cast<int>(n * n + 1)) {}

void Percolation::open(std::size_t row, std::size_t col) {
    check_bounds(row, col);
    if (is_open(row, col)) return;

    int idx = get_index(row, col);
    opened[idx] = true;
    open_sites_count++;

    if (row == 0) {
        dsu_perc.unite(idx, virtual_top);
        dsu_full.unite(idx, virtual_top);
    }
    if (row == n - 1) {
        dsu_perc.unite(idx, virtual_bottom);
    }

    int dRow[] = {-1, 1, 0, 0};
    int dCol[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i) {
        std::size_t newRow = row + dRow[i];
        std::size_t newCol = col + dCol[i];

        if (newRow < n && newCol < n && is_open(newRow, newCol)) {
            int neighbor_idx = get_index(newRow, newCol);
            dsu_perc.unite(idx, neighbor_idx);
            dsu_full.unite(idx, neighbor_idx);
        }
    }
}

bool Percolation::is_open(std::size_t row, std::size_t col) const {
    check_bounds(row, col);
    return opened[get_index(row, col)];
}

bool Percolation::is_full(std::size_t row, std::size_t col) {
    check_bounds(row, col);
    return is_open(row, col) && dsu_full.connected(get_index(row, col), virtual_top);
}

std::size_t Percolation::get_number_of_open_cells() const {
    return open_sites_count;
}

bool Percolation::percolates() {
    if (n == 0) return false;
    return dsu_perc.connected(virtual_top, virtual_bottom);
}