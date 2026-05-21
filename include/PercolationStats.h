#ifndef PERCOLATION_STATS_H
#define PERCOLATION_STATS_H

#include <cstddef>
#include <vector>

struct PercolationStats
{
    PercolationStats(size_t dimension, size_t trials);

    double get_mean() const;
    double get_standard_deviation() const;
    double get_confidence_low() const;
    double get_confidence_high() const;

    void execute();

    private:
        size_t dim;
        size_t num_trials;
        double mean;
        double std_dev;
        double conf_low;
        double conf_high;
        std::vector<double> thresholds;
};

#endif