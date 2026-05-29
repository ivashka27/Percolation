#pragma once
#include "Percolation.hpp"
#include <vector>
#include <random>
#include <cmath>

struct PercolationStats {
private:
    size_t dimension;
    size_t trials;
    std::vector<double> thresholds;
    double mean;
    double stddev;
    bool executed;
    
public:
    PercolationStats(size_t dimension, size_t trials);
    void execute();
    double get_mean() const;
    double get_standard_deviation() const;
    double get_confidence_low() const;
    double get_confidence_high() const;
};