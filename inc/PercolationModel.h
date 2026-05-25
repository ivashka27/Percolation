#pragma once
#include <vector>
#include "DSU.h"

class PercolationModel {
private:
    int grid_size;
    std::vector<bool> is_cell_open;
    DSU union_find;
    int virtual_top;
    int virtual_bottom;
    int open_cells_count;

    int flatten(int row, int col) const;
    bool is_within_bounds(int row, int col) const;

public:
    explicit PercolationModel(int size);

    void activate(int row, int col);
    bool is_cell_activated(int row, int col) const;
    bool is_connected_to_top(int row, int col);
    bool does_percolate();

    int get_number_open() const;
    int get_grid_size() const;
};
