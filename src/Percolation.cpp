#include "Percolation.hpp"
#include <random>
#include <cmath>
#include <algorithm>
#include <numeric>

// Реализация DSU
DSU::DSU(int n) {
    parent.resize(n);
    size.assign(n, 1);
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }
}

int DSU::find(int i) {
    if (parent[i] == i)
        return i;
    return parent[i] = find(parent[i]);
}

void DSU::unite(int i, int j) {
    int root_i = find(i);
    int root_j = find(j);
    if (root_i != root_j) {
        if (size[root_i] < size[root_j]) std::swap(root_i, root_j);
        parent[root_j] = root_i;
        size[root_i] += size[root_j];
    }
}

bool DSU::connected(int i, int j) {
    return find(i) == find(j);
}

Percolation::Percolation(size_t dimension) 
    : n(dimension), open_sites(n * n, false), dsu(n * n + 2), open_count(0) 
{
    virtual_top = n * n;
    virtual_bottom = n * n + 1;
}

int Percolation::get_index(int row, int col) const {
    return row * n + col;
}

void Percolation::open(int row, int col) {
    int idx = get_index(row, col);
    
    if (open_sites[idx]) return;

    open_sites[idx] = true;
    open_count++;

    if (row == 0) {
        dsu.unite(idx, virtual_top);
    }
    if (row == static_cast<int>(n) - 1) {
        dsu.unite(idx, virtual_bottom);
    }

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    
    for (int i = 0; i < 4; ++i) {
        int nr = row + dr[i];
        int nc = col + dc[i];
        
        if (nr >= 0 && nr < static_cast<int>(n) && nc >= 0 && nc < static_cast<int>(n)) {
            int neighbor_idx = get_index(nr, nc);
            if (open_sites[neighbor_idx]) {
                dsu.unite(idx, neighbor_idx);
            }
        }
    }
}

bool Percolation::is_open(int row, int col) const {
    return open_sites[get_index(row, col)];
}

bool Percolation::percolates() {
    return dsu.connected(virtual_top, virtual_bottom);
}

size_t Percolation::get_open_count() const {
    return open_count;
}

// Реализация PercolationStats (Монте-Карло)
PercolationStats::PercolationStats(size_t dimension, size_t trials)
    : dimension(dimension), trials(trials), mean_val(0.0), std_dev_val(0.0) {}

void PercolationStats::execute() {
    std::vector<double> thresholds(trials);
    std::random_device rd;
    std::mt19937 gen(rd());

    for (size_t t = 0; t < trials; ++t) {
        Percolation perc(dimension);
        
        std::vector<int> cells(dimension * dimension);
        std::iota(cells.begin(), cells.end(), 0);
        
        std::shuffle(cells.begin(), cells.end(), gen);

        size_t idx = 0;
        while (!perc.percolates() && idx < cells.size()) {
            int row = cells[idx] / dimension;
            int col = cells[idx] % dimension;
            perc.open(row, col);
            idx++;
        }

        thresholds[t] = static_cast<double>(perc.get_open_count()) / (dimension * dimension);
    }

    double sum = 0.0;
    for (double x : thresholds) {
        sum += x;
    }
    mean_val = sum / trials; //

    double sum_sq_diff = 0.0;
    for (double x : thresholds) {
        sum_sq_diff += (x - mean_val) * (x - mean_val);
    }
    
    if (trials > 1) {
        double variance = sum_sq_diff / (trials - 1); //
        std_dev_val = std::sqrt(variance);
    } else {
        std_dev_val = 0.0;
    }
}

double PercolationStats::get_mean() const { return mean_val; }
double PercolationStats::get_standard_deviation() const { return std_dev_val; }

double PercolationStats::get_confidence_low() const {
    return mean_val - (1.96 * std_dev_val) / std::sqrt(trials); //
}

double PercolationStats::get_confidence_high() const {
    return mean_val + (1.96 * std_dev_val) / std::sqrt(trials); //
}