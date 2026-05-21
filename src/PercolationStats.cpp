#include "PercolationStats.h"
#include "Percolation.h"
#include <numeric>
#include <cmath>
#include <random>
#include <algorithm>

PercolationStats::PercolationStats(size_t dimension, size_t trials): 
    dim(dimension), 
    num_trials(trials), 
    mean(0.0), 
    std_dev(0.0), 
    conf_low(0.0), 
    conf_high(0.0) {
        thresholds.resize(trials, 0.0);
}

double PercolationStats::get_mean() const { 
    return mean; 
}

double PercolationStats::get_standard_deviation() const { 
    return std_dev; 
}

double PercolationStats::get_confidence_low() const { 
    return conf_low; 
}

double PercolationStats::get_confidence_high() const { 
    return conf_high; 
}

void PercolationStats::execute() {
    if (num_trials == 0 || dim == 0) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::vector<size_t> indices(dim * dim);
    std::iota(indices.begin(), indices.end(), 0);

    for (size_t t = 0; t < num_trials; ++t) {
        Percolation perc(dim);
        std::shuffle(indices.begin(), indices.end(), gen);
        for (size_t idx : indices) {
            size_t row = idx / dim;
            size_t col = idx % dim;
            perc.open(row, col);
            if (perc.percolates()) {
                break;
            }
        }
        thresholds[t] = static_cast<double>(perc.get_open_sites_count()) / (dim * dim);
    }
    double sum = std::accumulate(thresholds.begin(), thresholds.end(), 0.0);
    mean = sum / num_trials;

    if (num_trials > 1) {
        double variance_sum = 0.0;
        for (double x : thresholds) {
            variance_sum += (x - mean) * (x - mean);
        }
        std_dev = std::sqrt(variance_sum / (num_trials - 1));
    } else {
        std_dev = 0.0;
    }

    double confidence_fraction = (1.96 * std_dev) / std::sqrt(num_trials);
    conf_low = mean - confidence_fraction;
    conf_high = mean + confidence_fraction;
}
