#pragma once

#include <vector>
#include <cstddef>

// Система непересекающихся множеств
class DSU {
private:
    std::vector<int> parent;
    std::vector<int> size;
public:
    DSU(int n);
    int find(int i);
    void unite(int i, int j);
    bool connected(int i, int j);
};

// Моделирование решетки
class Percolation {
private:
    size_t n;
    std::vector<bool> open_sites;
    DSU dsu;
    size_t open_count;
    int virtual_top;
    int virtual_bottom;

    int get_index(int row, int col) const;

public:
    explicit Percolation(size_t dimension);
    void open(int row, int col);
    bool is_open(int row, int col) const;
    bool percolates();
    size_t get_open_count() const;
};

struct PercolationStats {
private:
    size_t dimension;
    size_t trials;
    double mean_val;
    double std_dev_val;

public:
    /**
     * Construct a new Percolation Stats object
     * @param dimension dimension of percolation grid
     * @param trials amount of experiments
     */
    PercolationStats(size_t dimension, size_t trials);

    /**
     * Returns mean of percolation threshold (x¯ from description)
     */
    double get_mean() const;

    /**
     * Returns standard deviation of percolation threshold (s from description)
     */
    double get_standard_deviation() const;

    /**
     * Returns log edge of condidence interval
     */
    double get_confidence_low() const;

    /**
     * Returns high edge of confidence interval
     */
    double get_confidence_high() const;

    /**
     * Makes all experiments, calculates statistic values
     */
    void execute();
};