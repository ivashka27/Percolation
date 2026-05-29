#pragma once
#include <vector>
#include <cstddef>

struct PercolationStats
{
private:
    size_t dim;
    size_t t_trials;
    double mean_val;
    double std_dev;
    double conf_low;
    double conf_high;
    std::vector<double> results;

public:
    PercolationStats(size_t dimension, size_t trials);

    double get_mean() const;

    double get_standard_deviation() const;

    double get_confidence_low() const;

    double get_confidence_high() const;

    void execute();
};