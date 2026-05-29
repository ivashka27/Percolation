#include "..//include//percolation.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <algorithm>


Percolation::Percolation(size_t dimension) : n(dimension), open_cells_count(0) {
    size_t total_cells = n * n;
    opened.resize(total_cells, false);
    
    parent.resize(total_cells + 2);
    sz.resize(total_cells + 2, 1);

    for (size_t i = 0; i < parent.size(); ++i) {
        parent[i] = i;
    }

    virtual_top = 0;
    virtual_bottom = total_cells + 1;
}

size_t Percolation::get_index(size_t row, size_t col) const {
    return row * n + col + 1;
}

int Percolation::find_set(int v) {
    if (v == parent[v])
        return v;
    return parent[v] = find_set(parent[v]);
}

void Percolation::union_sets(int a, int b) {
    a = find_set(a);
    b = find_set(b);
    if (a != b) {
        if (sz[a] < sz[b])
            std::swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
    }
}

void Percolation::open(size_t row, size_t col) {
    size_t cell_idx = row * n + col;
    if (opened[cell_idx]) return;

    opened[cell_idx] = true;
    open_cells_count++;

    size_t current_dsu_idx = get_index(row, col);

    if (row == 0) {
        union_sets(virtual_top, current_dsu_idx);
    }

    if (row == n - 1) {
        union_sets(virtual_bottom, current_dsu_idx);
    }

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; ++i) {
        int new_r = static_cast<int>(row) + dr[i];
        int new_c = static_cast<int>(col) + dc[i];

        if (new_r >= 0 && new_r < static_cast<int>(n) && new_c >= 0 && new_c < static_cast<int>(n)) {
            size_t neighbor_cell_idx = new_r * n + new_c;
            if (opened[neighbor_cell_idx]) {
                union_sets(current_dsu_idx, get_index(new_r, new_c));
            }
        }
    }
}

bool Percolation::is_open(size_t row, size_t col) const {
    return opened[row * n + col];
}

bool Percolation::percolates() {
    return find_set(virtual_top) == find_set(virtual_bottom);
}

size_t Percolation::get_open_cells_count() const {
    return open_cells_count;
}


PercolationStats::PercolationStats(size_t dimension, size_t trials) 
    : dim(dimension), t(trials) {
    std::srand(std::time(nullptr));
}

void PercolationStats::execute() {
    results.clear();
    double total_cells = static_cast<double>(dim * dim);

    for (size_t i = 0; i < t; ++i) {
        Percolation perc(dim);

        std::vector<std::pair<size_t, size_t>> closed_cells;
        closed_cells.reserve(dim * dim);
        for (size_t r = 0; r < dim; ++r) {
            for (size_t c = 0; c < dim; ++c) {
                closed_cells.push_back({r, c});
            }
        }

        while (!perc.percolates()) {
            int rand_idx = std::rand() % closed_cells.size();
            auto [r, c] = closed_cells[rand_idx];
            
            closed_cells[rand_idx] = closed_cells.back();
            closed_cells.pop_back();

            perc.open(r, c);
        }

        double x_t = static_cast<double>(perc.get_open_cells_count()) / total_cells;
        results.push_back(x_t);
    }
}

double PercolationStats::get_mean() const {
    double sum = 0.0;
    for (double val : results) {
        sum += val;
    }
    return sum / t;
}

double PercolationStats::get_standard_deviation() const {
    if (t <= 1) return 0.0;
    
    double mean = get_mean();
    double sum_sq_diff = 0.0;
    for (double val : results) {
        sum_sq_diff += (val - mean) * (val - mean);
    }
    return std::sqrt(sum_sq_diff / (t - 1));
}

double PercolationStats::get_confidence_low() const {
    return get_mean() - (1.96 * get_standard_deviation()) / std::sqrt(t);
}

double PercolationStats::get_confidence_high() const {
    return get_mean() + (1.96 * get_standard_deviation()) / std::sqrt(t);
}