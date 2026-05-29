#include "percolation_stats.h"
#include "percolation.h"
#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>

PercolationStats::PercolationStats(size_t dimension, size_t trials)
    : dimension(dimension),
      trials(trials),
      mean(0.0),
      standard_deviation(0.0),
      confidence_low(0.0),
      confidence_high(0.0) {
}

void PercolationStats::execute() {
    results.clear();
    std::random_device rd;
    std::mt19937 generator(rd());
    for (size_t t = 0; t < trials; ++t) {
        Percolation percolation(dimension);
        std::vector<size_t> cells(dimension * dimension);
        for (size_t i = 0; i < cells.size(); ++i) {
            cells[i] = i;
        }
        std::shuffle(cells.begin(), cells.end(), generator);
        for (size_t cell : cells) {
            size_t row = cell / dimension;
            size_t col = cell % dimension;
            percolation.open(row, col);
            if (percolation.percolates()) {
                break;
            }
        }

        double threshold =
            static_cast<double>(percolation.get_opened_count()) /
            static_cast<double>(dimension * dimension);
        results.push_back(threshold);
    }

    mean = calculate_mean();
    standard_deviation = calculate_standard_deviation();
    double delta = 1.96 * standard_deviation / std::sqrt(static_cast<double>(trials));
    confidence_low = mean - delta;
    confidence_high = mean + delta;
}

double PercolationStats::calculate_mean() const {
    if (results.empty()) {
        return 0.0;
    }
    double sum = std::accumulate(results.begin(), results.end(), 0.0);
    return sum / static_cast<double>(results.size());
}

double PercolationStats::calculate_standard_deviation() const {
    if (results.size() <= 1) {
        return 0.0;
    }
    double sum = 0.0;
    for (double value : results) {
        double diff = value - mean;
        sum += diff * diff;
    }
    return std::sqrt(sum / static_cast<double>(results.size() - 1));
}

double PercolationStats::get_mean() const {
    return mean;
}

double PercolationStats::get_standard_deviation() const {
    return standard_deviation;
}

double PercolationStats::get_confidence_low() const {
    return confidence_low;
}

double PercolationStats::get_confidence_high() const {
    return confidence_high;
}