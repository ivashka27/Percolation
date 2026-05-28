#include "percolation/PercolationStats.hpp"

#include "percolation/Percolation.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>

PercolationStats::PercolationStats(std::size_t dimension, std::size_t trials)
    : dimension_(dimension),
      trials_(trials),
      mean_(0.0),
      standard_deviation_(0.0),
      confidence_low_(0.0),
      confidence_high_(0.0),
      random_engine_(std::random_device{}())
{
    if (dimension == 0) {
        throw std::invalid_argument("PercolationStats dimension must be greater than zero");
    }
    if (trials < 2) {
        throw std::invalid_argument("PercolationStats trials must be at least two");
    }
}

double PercolationStats::get_mean() const
{
    return mean_;
}

double PercolationStats::get_standard_deviation() const
{
    return standard_deviation_;
}

double PercolationStats::get_confidence_low() const
{
    return confidence_low_;
}

double PercolationStats::get_confidence_high() const
{
    return confidence_high_;
}

void PercolationStats::execute()
{
    thresholds_.clear();
    thresholds_.reserve(trials_);

    for (std::size_t trial = 0; trial < trials_; ++trial) {
        thresholds_.push_back(run_trial());
    }

    calculate_statistics();
}

double PercolationStats::run_trial()
{
    Percolation percolation(dimension_);
    std::vector<std::size_t> sites(dimension_ * dimension_);
    std::iota(sites.begin(), sites.end(), 0);
    std::shuffle(sites.begin(), sites.end(), random_engine_);

    for (const std::size_t site : sites) {
        const std::size_t row = site / dimension_;
        const std::size_t col = site % dimension_;
        percolation.open(row, col);

        if (percolation.percolates()) {
            break;
        }
    }

    const double open_sites = static_cast<double>(percolation.get_open_sites_count());
    const double total_sites = static_cast<double>(dimension_ * dimension_);
    return open_sites / total_sites;
}

void PercolationStats::calculate_statistics()
{
    const double trials = static_cast<double>(trials_);
    mean_ = std::accumulate(thresholds_.begin(), thresholds_.end(), 0.0) / trials;

    double variance_sum = 0.0;
    for (const double threshold : thresholds_) {
        const double difference = threshold - mean_;
        variance_sum += difference * difference;
    }

    standard_deviation_ = std::sqrt(variance_sum / static_cast<double>(trials_ - 1));

    const double confidence_delta = 1.96 * standard_deviation_ / std::sqrt(trials);
    confidence_low_ = mean_ - confidence_delta;
    confidence_high_ = mean_ + confidence_delta;
}
