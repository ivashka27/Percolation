#include "PercolationStats.h"
#include <random>
#include <cmath>
#include <numeric>

PercolationStats::PercolationStats(size_t dimension, size_t trials)
    : n_(dimension), trials_(trials), mean_(0), stddev_(0), conf_low_(0), conf_high_(0) {}
void PercolationStats::execute() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dis(0, n_-1);
    thresholds_.clear();
    for (size_t t = 0; t < trials_; ++t) {
        Percolation perc(n_);
        while (!perc.percolates()) {
            size_t r = dis(gen);
            size_t c = dis(gen);
            perc.open(r, c);
        }
        thresholds_.push_back(double(perc.numberOfOpenSites()) / (n_ * n_));
    }
    mean_ = std::accumulate(thresholds_.begin(), thresholds_.end(), 0.0) / thresholds_.size();
    double sq_sum = 0.0;
    for (double x : thresholds_) sq_sum += (x - mean_) * (x - mean_);
    stddev_ = std::sqrt(sq_sum / (thresholds_.size() - 1));
    double margin = 1.96 * stddev_ / std::sqrt(trials_);
    conf_low_ = mean_ - margin;
    conf_high_ = mean_ + margin;
}
double PercolationStats::get_mean() const { return mean_; }
double PercolationStats::get_standard_deviation() const { return stddev_; }
double PercolationStats::get_confidence_low() const { return conf_low_; }
double PercolationStats::get_confidence_high() const { return conf_high_; }