#ifndef PERCOLATIONSTATS_H
#define PERCOLATIONSTATS_H
#include "Percolation.h"
#include <vector>

class PercolationStats {
public:
    PercolationStats(size_t dimension, size_t trials);
    void execute();
    double get_mean() const;
    double get_standard_deviation() const;
    double get_confidence_low() const;
    double get_confidence_high() const;
private:
    size_t n_;
    size_t trials_;
    std::vector<double> thresholds_;
    double mean_;
    double stddev_;
    double conf_low_;
    double conf_high_;
};
#endif