#include "../inc/PercolationModel.h"

PercolationModel::PercolationModel(int size)
    : grid_size(size)
    , is_cell_open(size * size, false)
    , union_find(size * size + 2)
    , virtual_top(size * size)
    , virtual_bottom(size * size + 1)
    , open_cells_count(0) {}

int PercolationModel::flatten(int row, int col) const {
    return row * grid_size + col;
}

bool PercolationModel::is_within_bounds(int row, int col) const {
    return row >= 0 && row < grid_size && col >= 0 && col < grid_size;
}

void PercolationModel::activate(int row, int col) {
    if (!is_within_bounds(row, col)) return;

    int current_idx = flatten(row, col);
    if (is_cell_open[current_idx]) return;

    is_cell_open[current_idx] = true;
    open_cells_count++;

    if (row == 0) {
        union_find.merge(current_idx, virtual_top);
    }

    if (row == grid_size - 1) {
        union_find.merge(current_idx, virtual_bottom);
    }

    if (row > 0 && is_cell_activated(row - 1, col)) {
        union_find.merge(current_idx, flatten(row - 1, col));
    }
    if (row + 1 < grid_size && is_cell_activated(row + 1, col)) {
        union_find.merge(current_idx, flatten(row + 1, col));
    }
    if (col > 0 && is_cell_activated(row, col - 1)) {
        union_find.merge(current_idx, flatten(row, col - 1));
    }
    if (col + 1 < grid_size && is_cell_activated(row, col + 1)) {
        union_find.merge(current_idx, flatten(row, col + 1));
    }
}

bool PercolationModel::is_cell_activated(int row, int col) const {
    if (!is_within_bounds(row, col)) return false;
    return is_cell_open[flatten(row, col)];
}

bool PercolationModel::is_connected_to_top(int row, int col) {
    if (!is_cell_activated(row, col)) return false;
    return union_find.is_connected(flatten(row, col), virtual_top);
}

bool PercolationModel::does_percolate() {
    return union_find.is_connected(virtual_top, virtual_bottom);
}

int PercolationModel::get_number_open() const {
    return open_cells_count;
}

int PercolationModel::get_grid_size() const {
    return grid_size;
}
