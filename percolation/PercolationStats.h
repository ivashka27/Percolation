#pragma once

#include <cstddef>
#include <vector>

struct PercolationStats {
    PercolationStats(size_t dimension, size_t trials);

    double get_mean() const;
    double get_standard_deviation() const;
    double get_confidence_low() const;
    double get_confidence_high() const;

    void execute();

private:
    size_t dimension_;
    size_t trials_;
    std::vector<double> thresholds_;
    double mean_;
    double standard_deviation_;
    double confidence_low_;
    double confidence_high_;
    bool executed_;
};
