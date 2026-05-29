#include "PercolationStats.h"
#include "Percolation.h"
#include <cmath>
#include <random>
#include <algorithm>

PercolationStats::PercolationStats(size_t dimension, size_t trials) 
    : dim(dimension), t_trials(trials), mean_val(0.0), std_dev(0.0), conf_low(0.0), conf_high(0.0) 
{
    results.resize(trials, 0.0);
}

double PercolationStats::get_mean() const {
    return mean_val;
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
    if (t_trials == 0 || dim == 0) return;

    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<std::pair<size_t, size_t>> coordinates;
    coordinates.reserve(dim * dim);
    for (size_t r = 0; r < dim; ++r) {
        for (size_t c = 0; c < dim; ++c) {
            coordinates.push_back({r, c});
        }
    }

    for (size_t t = 0; t < t_trials; ++t) {
        Percolation perc(dim);
        
        std::shuffle(coordinates.begin(), coordinates.end(), gen);

        for (const auto& cell : coordinates) {
            perc.open(cell.first, cell.second);
            if (perc.percolates()) {
                break;
            }
        }

        double total_cells = static_cast<double>(dim * dim);
        results[t] = static_cast<double>(perc.get_open_sites_count()) / total_cells;
    }

    double sum = 0.0;
    for (double val : results) {
        sum += val;
    }
    mean_val = sum / t_trials;

    if (t_trials > 1) {
        double variance_sum = 0.0;
        for (double val : results) {
            variance_sum += (val - mean_val) * (val - mean_val);
        }
        std_dev = std::sqrt(variance_sum / (t_trials - 1));
    } else {
        std_dev = 0.0;
    }

    double margin = (1.96 * std_dev) / std::sqrt(static_cast<double>(t_trials));
    conf_low = mean_val - margin;
    conf_high = mean_val + margin;
}