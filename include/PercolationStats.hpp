#pragma once

#include <cstddef>
#include <vector>

struct PercolationStats {
    PercolationStats(std::size_t dimension, std::size_t trials);

    double get_mean() const;
    double get_standard_deviation() const;
    double get_confidence_low() const;
    double get_confidence_high() const;
    void execute();

private:
    std::size_t dim;
    std::size_t t;
    double mean_val;
    double std_dev_val;
    std::vector<double> thresholds;
};