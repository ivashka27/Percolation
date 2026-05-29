#ifndef PERCOLATION_HPP
#define PERCOLATION_HPP

#include <vector>
#include <cstddef>

class Percolation {
private:
    size_t n;
    std::vector<bool> opened;
    std::vector<int> parent;
    std::vector<int> sz;

    size_t virtual_top;
    size_t virtual_bottom;
    size_t open_cells_count;

    int find_set(int v);
    void union_sets(int a, int b);
    size_t get_index(size_t row, size_t col) const;

public:
Percolation(size_t dimension);
    void open(size_t row, size_t col);
    bool is_open(size_t row, size_t col) const;
    bool percolates();
    size_t get_open_cells_count() const;
};

struct PercolationStats {
    size_t dim;
    size_t t;
    std::vector<double> results;

    PercolationStats(size_t dimension, size_t trials);

    double get_mean() const;

    double get_standard_deviation() const;

    double get_confidence_low() const;

    double get_confidence_high() const;

    void execute();
};

#endif