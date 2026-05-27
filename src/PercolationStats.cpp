#include "PercolationStats.hpp"
#include "Percolation.hpp"
#include <stdexcept>
#include <random>
#include <algorithm>
#include <cmath>
#include <numeric>

PercolationStats::PercolationStats(std::size_t dimension, std::size_t trials) : 
    dim(dimension), trials_count(trials), mean_val(0), stddev_val(0) {
    if (dimension <= 0 || trials <= 0) {
        throw std::invalid_argument("Размерность и количество экспериментов должны быть > 0");
    }
}

void PercolationStats::execute() {
    thresholds.clear();
    thresholds.reserve(trials_count);
    
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<int> all_sites(dim * dim);
    std::iota(all_sites.begin(), all_sites.end(), 0);

    double sum = 0.0;

    for (std::size_t t = 0; t < trials_count; ++t) {
        Percolation perc(dim);
        
        std::shuffle(all_sites.begin(), all_sites.end(), gen);
        
        std::size_t idx = 0;
        while (!perc.percolates()) {
            int row = all_sites[idx] / dim;
            int col = all_sites[idx] % dim;
            perc.open(row, col);
            idx++;
        }
        
        double threshold = static_cast<double>(perc.get_number_of_open_cells()) / (dim * dim);
        thresholds.push_back(threshold);
        sum += threshold;
    }

    mean_val = sum / trials_count;

    double sum_sq_diff = 0.0;
    for (double val : thresholds) {
        sum_sq_diff += (val - mean_val) * (val - mean_val);
    }
    
    if (trials_count > 1) {
        stddev_val = std::sqrt(sum_sq_diff / (trials_count - 1));
    } else {
        stddev_val = std::numeric_limits<double>::quiet_NaN();
    }
}

double PercolationStats::get_mean() const {
    return mean_val;
}

double PercolationStats::get_standard_deviation() const {
    return stddev_val;
}

double PercolationStats::get_confidence_low() const {
    return mean_val - (1.96 * stddev_val) / std::sqrt(trials_count);
}

double PercolationStats::get_confidence_high() const {
    return mean_val + (1.96 * stddev_val) / std::sqrt(trials_count);
}