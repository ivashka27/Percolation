#pragma once
#include <vector>
#include <cstddef>

class PercolationStats {
private:
    size_t dimension;
    size_t trials;
    std::vector<double> results;
    double mean;
    double standard_deviation;
    double confidence_low;
    double confidence_high;
    double calculate_mean() const;
    double calculate_standard_deviation() const;

public:
    PercolationStats(size_t dimension, size_t trials);
    void execute();
    double get_mean() const;
    double get_standard_deviation() const;
    double get_confidence_low() const;
    double get_confidence_high() const;
};