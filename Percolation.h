#pragma once
#include "DSU.h"
#include <vector>

class Percolation {
private:
    size_t grid_side;
    std::vector<bool> grid_state;
    DSU uf_solver;

    size_t top_node;
    size_t bottom_node;
    size_t active_cells;

    size_t calculate_index(size_t row, size_t col) const {
        return (row * grid_side) + col + 1;
    }

public:
    explicit Percolation(size_t dimension)
        : grid_side(dimension),
          grid_state(dimension * dimension, false),
          uf_solver(dimension * dimension + 2),
          top_node(0),
          bottom_node(dimension * dimension + 1),
          active_cells(0) {}

    void open_cell(size_t row, size_t col) {
        size_t state_idx = row * grid_side + col;
        if (grid_state[state_idx]) return;

        grid_state[state_idx] = true;
        active_cells++;

        size_t current_uf_idx = calculate_index(row, col);

        if (row == 0) {
            uf_solver.merge(current_uf_idx, top_node);
        }
        if (row == grid_side - 1) {
            uf_solver.merge(current_uf_idx, bottom_node);
        }

        if (row > 0 && is_cell_open(row - 1, col)) {
            uf_solver.merge(current_uf_idx, calculate_index(row - 1, col));
        }
        if (row < grid_side - 1 && is_cell_open(row + 1, col)) {
            uf_solver.merge(current_uf_idx, calculate_index(row + 1, col));
        }
        if (col > 0 && is_cell_open(row, col - 1)) {
            uf_solver.merge(current_uf_idx, calculate_index(row, col - 1));
        }
        if (col < grid_side - 1 && is_cell_open(row, col + 1)) {
            uf_solver.merge(current_uf_idx, calculate_index(row, col + 1));
        }
    }

    bool is_cell_open(size_t row, size_t col) const {
        return grid_state[row * grid_side + col];
    }

    size_t get_active_cells_count() const {
        return active_cells;
    }

    bool system_percolates() {
        return uf_solver.are_connected(top_node, bottom_node);
    }
};