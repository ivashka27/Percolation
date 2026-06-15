#include "PercolationStats.hpp"
#include "Percolation.hpp"
#include <cmath>
#include <random>
#include <numeric>
#include <algorithm>

PercolationStats::PercolationStats(std::size_t dimension, std::size_t trials) 
    : dim(dimension), t(trials), mean_val(0.0), std_dev_val(0.0) {
    thresholds.reserve(t);
}

void PercolationStats::execute() {
    std::mt19937 gen(std::random_device{}());
    std::vector<int> order(dim * dim);
    std::iota(order.begin(), order.end(), 0);

    for (std::size_t i = 0; i < t; ++i) {
        Percolation perc(dim);
        std::shuffle(order.begin(), order.end(), gen);

        for (int idx : order) {
            std::size_t row = idx / dim;
            std::size_t col = idx % dim;
            perc.open(row, col);

            if (perc.percolates()) {
                double threshold = static_cast<double>(perc.number_of_open_sites()) / (dim * dim);
                thresholds.push_back(threshold);
                break;
            }
        }
    }

    double sum = std::accumulate(thresholds.begin(), thresholds.end(), 0.0);
    mean_val = sum / t;

    double sq_sum = 0.0;
    for (double val : thresholds) {
        sq_sum += (val - mean_val) * (val - mean_val);
    }
    
    if (t > 1) {
        std_dev_val = std::sqrt(sq_sum / (t - 1));
    } else {
        std_dev_val = 0.0;
    }
}

double PercolationStats::get_mean() const {
    return mean_val;
}

double PercolationStats::get_standard_deviation() const {
    return std_dev_val;
}

double PercolationStats::get_confidence_low() const {
    return mean_val - (1.96 * std_dev_val) / std::sqrt(t);
}

double PercolationStats::get_confidence_high() const {
    return mean_val + (1.96 * std_dev_val) / std::sqrt(t);
}