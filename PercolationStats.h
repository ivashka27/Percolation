#pragma once

#include <vector>
#include <cstddef>

struct PercolationStats
{
private:
    size_t dimension;
    size_t trials;

    std::vector<double> results;

    double mean;
    double standard_deviation;
    double confidence_low;
    double confidence_high;

    bool executed;

    void calculate_statistics();

public:
    /**
     * Construct a new Percolation Stats object
     * @param dimension dimension of percolation grid
     * @param trials amount of experiments
     */
    PercolationStats(size_t dimension, size_t trials);

    /**
     * Returns mean of percolation threshold
     */
    double get_mean() const;

    /**
     * Returns standard deviation of percolation threshold
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
};