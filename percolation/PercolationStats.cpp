#include "PercolationStats.h"

#include "Percolation.h"

#include <cmath>
#include <limits>
#include <numeric>
#include <random>
#include <stdexcept>
#include <vector>

namespace {
constexpr double kConfidenceFactor = 1.96;
}

PercolationStats::PercolationStats(size_t dimension, size_t trials)
    : dimension_(dimension),
      trials_(trials),
      mean_(std::numeric_limits<double>::quiet_NaN()),
      standard_deviation_(std::numeric_limits<double>::quiet_NaN()),
      confidence_low_(std::numeric_limits<double>::quiet_NaN()),
      confidence_high_(std::numeric_limits<double>::quiet_NaN()),
      executed_(false) {
    if (dimension_ == 0) {
        throw std::invalid_argument("dimension must be positive");
    }
    if (trials_ == 0) {
        throw std::invalid_argument("trials must be positive");
    }
}

double PercolationStats::get_mean() const {
    if (!executed_) {
        throw std::logic_error("call execute() before reading stats");
    }
    return mean_;
}

double PercolationStats::get_standard_deviation() const {
    if (!executed_) {
        throw std::logic_error("call execute() before reading stats");
    }
    return standard_deviation_;
}

double PercolationStats::get_confidence_low() const {
    if (!executed_) {
        throw std::logic_error("call execute() before reading stats");
    }
    return confidence_low_;
}

double PercolationStats::get_confidence_high() const {
    if (!executed_) {
        throw std::logic_error("call execute() before reading stats");
    }
    return confidence_high_;
}

void PercolationStats::execute() {
    thresholds_.clear();
    thresholds_.reserve(trials_);

    const size_t total_sites = dimension_ * dimension_;
    std::mt19937 rng(std::random_device{}());

    for (size_t trial = 0; trial < trials_; ++trial) {
        Percolation percolation(dimension_);
        std::vector<size_t> closed_sites(total_sites);
        std::iota(closed_sites.begin(), closed_sites.end(), 0);
        size_t remaining = total_sites;

        while (!percolation.percolates()) {
            std::uniform_int_distribution<size_t> pick(0, remaining - 1);
            const size_t choice = pick(rng);
            const size_t site = closed_sites[choice];
            std::swap(closed_sites[choice], closed_sites[remaining - 1]);
            --remaining;

            percolation.open(site / dimension_, site % dimension_);
        }

        thresholds_.push_back(
            static_cast<double>(percolation.get_open_sites_count()) / total_sites);
    }

    mean_ = std::accumulate(thresholds_.begin(), thresholds_.end(), 0.0) / trials_;

    if (trials_ == 1) {
        standard_deviation_ = std::numeric_limits<double>::quiet_NaN();
        confidence_low_ = std::numeric_limits<double>::quiet_NaN();
        confidence_high_ = std::numeric_limits<double>::quiet_NaN();
        executed_ = true;
        return;
    }

    double squared_sum = 0.0;
    for (double value : thresholds_) {
        const double diff = value - mean_;
        squared_sum += diff * diff;
    }

    standard_deviation_ = std::sqrt(squared_sum / (trials_ - 1));
    const double margin = kConfidenceFactor * standard_deviation_ / std::sqrt(trials_);
    confidence_low_ = mean_ - margin;
    confidence_high_ = mean_ + margin;
    executed_ = true;
}
