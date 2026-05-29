#pragma once

#include <cstddef>
#include <vector>

struct PercolationStats
{
    PercolationStats(std::size_t dimension, std::size_t trials);

    double get_mean() const;
    double get_standard_deviation() const;
    double get_confidence_low() const;
    double get_confidence_high() const;

    void execute();

private:
    double run_one_trial();
    void calculate_values();

    std::size_t grid_dimension;
    std::size_t experiment_count;
    std::vector<double> thresholds;
    double mean_value;
    double standard_deviation;
    double confidence_low;
    double confidence_high;
};
