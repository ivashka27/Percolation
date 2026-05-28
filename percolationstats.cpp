#include "PercolationStats.h"
#include "Percolation.h"
#include <random>
#include <cmath>
#include <vector>

PercolationStats::PercolationStats(size_t dim, size_t t)
    : dimension(dim)
    , trials(t)
    , mean(0.0)
    , stddev(0.0)
    , confidence_low(0.0)
    , confidence_high(0.0)
{}

void PercolationStats::execute() {
    std::vector<double> thresholds(trials);
    
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<size_t> dist(0, dimension - 1);

    for (size_t t = 0; t < trials; ++t) {
        Percolation perc(dimension);

        while (!perc.has_percolation()) {
            size_t row = dist(rng);
            size_t col = dist(rng);

            if (!perc.is_open(row, col)) {
                perc.open(row, col);
            }
        }

        thresholds[t] = static_cast<double>(perc.number_of_open_cells()) 
                      / (dimension * dimension);
    }

    // среднее
    double sum = 0.0;
    for (size_t i = 0; i < trials; ++i) {
        sum += thresholds[i];
    }
    mean = sum / trials;

    // стандартное отклонение
    double sq_sum = 0.0;
    for (size_t i = 0; i < trials; ++i) {
        double diff = thresholds[i] - mean;
        sq_sum += diff * diff;
    }
    stddev = std::sqrt(sq_sum / (trials - 1));

    // 95% доверительный интервал
    double margin = 1.96 * stddev / std::sqrt(static_cast<double>(trials));
    confidence_low = mean - margin;
    confidence_high = mean + margin;
}

double PercolationStats::get_mean() const {
    return mean;
}

double PercolationStats::get_standard_deviation() const {
    return stddev;
}

double PercolationStats::get_confidence_low() const {
    return confidence_low;
}

double PercolationStats::get_confidence_high() const {
    return confidence_high;
}