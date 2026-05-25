#pragma once
#include "percolation.hpp"
#include <vector>
#include <cmath>
#include <random>
#include <stdexcept>

struct PercolationStats {
    PercolationStats(size_t dimension, size_t trials) : dimension_(dimension), trials_(trials), mean_(0.0), std_dev_(0.0) {
        if (dimension == 0) throw std::invalid_argument("dimension must be > 0");
        if (trials == 0) throw std::invalid_argument("trials must be > 0");
    }

    double get_mean() const {
        return mean_;
    }

    double get_standard_deviation() const {
        return std_dev_;
    }

    double get_confidence_low() const {
        return mean_ - 1.96 * std_dev_ / std::sqrt(static_cast<double>(trials_));
    }

    double get_confidence_high() const {
        return mean_ + 1.96 * std_dev_ / std::sqrt(static_cast<double>(trials_));
    }

    void execute() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dist(0, dimension_ - 1);

        std::vector<double> thresholds(trials_);

        for (size_t t = 0; t < trials_; ++t) {
            Percolation perc(dimension_);

            while (!perc.percolates()) {
                size_t row, col;
                do {
                    row = dist(gen);
                    col = dist(gen);
                } while (perc.is_open(row, col));
                perc.open_cell(row, col);
            }

            thresholds[t] = static_cast<double>(perc.number_of_open_sites()) / static_cast<double>(dimension_ * dimension_);
        }

        mean_ = 0.0;
        for (double x : thresholds) mean_ += x;
        mean_ /= static_cast<double>(trials_);

        double variance = 0.0;
        for (double x : thresholds) {
            double diff = x - mean_;
            variance += diff * diff;
        }
        std_dev_ = (trials_ > 1) ? std::sqrt(variance / static_cast<double>(trials_ - 1)) : 0.0;
    }

private:
    size_t dimension_;
    size_t trials_;
    double mean_;
    double std_dev_;
};