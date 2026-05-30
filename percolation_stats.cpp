#include "percolation_stats.h"
#include <random>
#include <algorithm>
#include <cmath>

PercolationStats::PercolationStats(size_t dimension, size_t trials)
    : dimension(dimension), trials(trials), percolation_thresholds(trials) {
}

void PercolationStats::execute() {
    std::random_device rd;
    std::mt19937 gen(rd());

    for (size_t t = 0; t < trials; ++t) {
        Percolation grid(static_cast<int>(dimension));

        std::vector<int> sites(dimension * dimension);
        for (size_t i = 0; i < sites.size(); ++i) {
            sites[i] = static_cast<int>(i);
        }

        std::shuffle(sites.begin(), sites.end(), gen);

        size_t opened_count = 0;
        for (int site_idx : sites) {
            int row = site_idx / static_cast<int>(dimension);
            int col = site_idx % static_cast<int>(dimension);

            grid.open(row, col);
            opened_count++;

            if (grid.percolates()) {
                break;
            }
        }

        percolation_thresholds[t] = static_cast<double>(opened_count) / static_cast<double>(dimension * dimension);
    }
}

double PercolationStats::get_mean() const {
    if (percolation_thresholds.empty()) return 0.0;

    double sum = 0.0;
    for (double p : percolation_thresholds) {
        sum += p;
    }
    return sum / static_cast<double>(trials);
}

double PercolationStats::get_standard_deviation() const {
    if (trials < 2) return 0.0;

    double mean = get_mean();
    double sum_sq_diff = 0.0;
    for (double p : percolation_thresholds) {
        sum_sq_diff += (p - mean) * (p - mean);
    }
    return std::sqrt(sum_sq_diff / static_cast<double>(trials - 1));
}

double PercolationStats::get_confidence_low() const {
    double mean = get_mean();
    double s = get_standard_deviation();
    return mean - (1.96 * s / std::sqrt(static_cast<double>(trials)));
}

double PercolationStats::get_confidence_high() const {
    double mean = get_mean();
    double s = get_standard_deviation();
    return mean + (1.96 * s / std::sqrt(static_cast<double>(trials)));
}
