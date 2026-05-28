#pragma once

#include "percolation.h"
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <stdexcept>

/**
 * Monte-Carlo simulation to estimate the percolation threshold.
 *
 * Algorithm for each trial:
 *   1. Create an n×n grid (all blocked).
 *   2. Randomly open cells one by one until the system percolates.
 *   3. Record x_t = (number of open cells) / (n * n).
 *
 * Statistics computed over T trials:
 *   mean       = x̄  = (Σ x_t) / T
 *   std_dev    = s   = sqrt(Σ(x_t - x̄)² / (T-1))
 *   confidence interval (95%) = [ x̄ - 1.96*s/√T,  x̄ + 1.96*s/√T ]
 */
struct PercolationStats {
    /**
     * @param dimension  size of the n×n grid
     * @param trials     number of independent experiments
     */
    PercolationStats(size_t dimension, size_t trials)
        : n_(dimension), trials_(trials), mean_(0), std_dev_(0)
    {
        if (dimension == 0) throw std::invalid_argument("dimension must be > 0");
        if (trials == 0)    throw std::invalid_argument("trials must be > 0");
        results_.reserve(trials);
    }

    /** Returns mean of percolation threshold (x̄). */
    double get_mean() const { return mean_; }

    /** Returns standard deviation of percolation threshold (s). */
    double get_standard_deviation() const { return std_dev_; }

    /** Returns lower bound of 95% confidence interval. */
    double get_confidence_low() const {
        return mean_ - 1.96 * std_dev_ / std::sqrt(static_cast<double>(trials_));
    }

    /** Returns upper bound of 95% confidence interval. */
    double get_confidence_high() const {
        return mean_ + 1.96 * std_dev_ / std::sqrt(static_cast<double>(trials_));
    }

    /** Runs all experiments and computes statistics. */
    void execute() {
        results_.clear();

        std::mt19937 rng(std::random_device{}());

        for (size_t t = 0; t < trials_; ++t) {
            results_.push_back(run_one_trial(rng));
        }

        // Compute mean
        double sum = 0.0;
        for (double x : results_) sum += x;
        mean_ = sum / static_cast<double>(trials_);

        // Compute standard deviation (sample)
        if (trials_ > 1) {
            double sq_sum = 0.0;
            for (double x : results_) {
                double diff = x - mean_;
                sq_sum += diff * diff;
            }
            std_dev_ = std::sqrt(sq_sum / static_cast<double>(trials_ - 1));
        } else {
            std_dev_ = 0.0;
        }
    }

private:
    size_t n_;
    size_t trials_;
    double mean_;
    double std_dev_;
    std::vector<double> results_;

    /**
     * Runs a single Monte-Carlo trial.
     * Returns the fraction of open cells when the system first percolates.
     */
    double run_one_trial(std::mt19937& rng) {
        Percolation perc(n_);
        size_t total = n_ * n_;

        // Build a shuffled list of all cell indices
        std::vector<size_t> indices(total);
        for (size_t i = 0; i < total; ++i) indices[i] = i;
        std::shuffle(indices.begin(), indices.end(), rng);

        for (size_t k = 0; k < total; ++k) {
            size_t row = indices[k] / n_;
            size_t col = indices[k] % n_;
            perc.open(row, col);
            if (perc.percolates()) {
                return static_cast<double>(perc.number_of_open_sites()) /
                       static_cast<double>(total);
            }
        }
        // Should not reach here for a valid grid, but just in case:
        return 1.0;
    }
};
