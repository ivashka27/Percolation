#pragma once
#include <vector>
#include <cstddef>

struct PercolationStats {
private:
    std::size_t dim;
    std::size_t trials_count;
    std::vector<double> thresholds;
    double mean_val;
    double stddev_val;

public:
    PercolationStats(std::size_t dimension, std::size_t trials);
    void execute();
    double get_mean() const;
    double get_standard_deviation() const;
    double get_confidence_low() const;
    double get_confidence_high() const;
};