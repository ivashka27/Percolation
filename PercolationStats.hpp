#pragma once

#include <cstddef>
#include <random>
#include <vector>

struct PercolationStats
{
    /**
     * Construct a new Percolation Stats object
     * @param dimension dimension of percolation grid
     * @param trials amount of experiments
     */
    PercolationStats(std::size_t dimension, std::size_t trials);

    /**
     * Returns mean of percolation threshold (x from description)
     */
    double get_mean() const;

    /**
     * Returns standard deviation of percolation threshold (s from description)
     */
    double get_standard_deviation() const;

    /**
     * Returns low edge of confidence interval
     */
    double get_confidence_low() const;

    /**
     * Returns high edge of confidence interval
     */
    double get_confidence_high() const;

    /**
     * Makes all experiments, calculates statistic values
     */
    void execute();

private:
    double run_trial();
    void calculate_statistics();

    std::size_t dimension_;
    std::size_t trials_;
    std::vector<double> thresholds_;
    double mean_;
    double standard_deviation_;
    double confidence_low_;
    double confidence_high_;
    std::mt19937_64 random_engine_;
};
