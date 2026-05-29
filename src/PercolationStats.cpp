#include "PercolationStats.hpp"
#include <stdexcept>

PercolationStats::PercolationStats(size_t dimension, size_t trials) 
    : dimension(dimension), 
      trials(trials),
      thresholds(trials),
      mean(0.0),
      stddev(0.0),
      executed(false) {}

void PercolationStats::execute() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, dimension - 1);
    
    for (size_t t = 0; t < trials; ++t) {
        Percolation perc(dimension);
        
        while (!perc.percolates()) {
            size_t row, col;
            do {
                row = dist(gen);
                col = dist(gen);
            } while (perc.is_open(row, col));
            
            perc.open(row, col);
        }
        thresholds[t] = static_cast<double>(perc.get_number_of_open_sites()) / (dimension * dimension);
    }
    
    double sum = 0.0;
    for (double threshold : thresholds) {
        sum += threshold;
    }
    mean = sum / trials;

    if (trials > 1) {
        double variance = 0.0;
        for (double threshold : thresholds) {
            double diff = threshold - mean;
            variance += diff * diff;
        }
        variance /= (trials - 1);
        stddev = std::sqrt(variance);
    } else {
        stddev = 0.0;
    }
    executed = true;
}

double PercolationStats::get_mean() const {
    if (!executed) throw std::runtime_error("Execute must be called first");
    return mean;
}

double PercolationStats::get_standard_deviation() const {
    if (!executed) throw std::runtime_error("Execute must be called first");
    return stddev;
}

double PercolationStats::get_confidence_low() const {
    if (!executed) throw std::runtime_error("Execute must be called first");
    return mean - 1.96 * stddev / std::sqrt(trials);
}

double PercolationStats::get_confidence_high() const {
    if (!executed) throw std::runtime_error("Execute must be called first");
    return mean + 1.96 * stddev / std::sqrt(trials);
}