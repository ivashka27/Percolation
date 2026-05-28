#pragma once

#include <vector>
#include <cstddef>

struct PercolationStats
{
    PercolationStats(size_t dimension, size_t trials);

    double get_mean() const;

    double get_standard_deviation() const;

    double get_confidence_low() const;

    double get_confidence_high() const;

    void execute();

private:
    size_t dimension;
    size_t trials;

    std::vector<double> results;

    double mean;
    double deviation;
};