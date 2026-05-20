#include "PercolationStats.h"

#include "Percolation.h"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>
#include <stdexcept>
#include <vector>

namespace
{
constexpr double CONFIDENCE_95_COEFFICIENT = 1.96;

double run_single_experiment(size_t dimension, std::mt19937& generator)
{
    Percolation percolation(dimension);

    std::vector<size_t> cells(dimension * dimension);
    std::iota(cells.begin(), cells.end(), 0);

    std::shuffle(cells.begin(), cells.end(), generator);

    for (size_t cell : cells)
    {
        const size_t row = cell / dimension;
        const size_t column = cell % dimension;

        percolation.open(row, column);

        if (percolation.percolates())
        {
            return static_cast<double>(percolation.get_open_count())
                / static_cast<double>(dimension * dimension);
        }
    }

    return 1.0;
}
}

PercolationStats::PercolationStats(size_t dimension, size_t trials)
    : dimension_(dimension),
      trials_(trials),
      thresholds_(),
      mean_(0.0),
      standard_deviation_(0.0),
      confidence_low_(0.0),
      confidence_high_(0.0)
{
    if (dimension == 0)
    {
        throw std::invalid_argument("dimension must be positive");
    }

    if (trials == 0)
    {
        throw std::invalid_argument("trials must be positive");
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

    std::random_device random_device;
    std::mt19937 generator(random_device());

    for (size_t trial = 0; trial < trials_; ++trial)
    {
        thresholds_.push_back(run_single_experiment(dimension_, generator));
    }

    const double sum = std::accumulate(thresholds_.begin(), thresholds_.end(), 0.0);
    mean_ = sum / static_cast<double>(trials_);

    double squared_sum = 0.0;

    for (double threshold : thresholds_)
    {
        const double difference = threshold - mean_;
        squared_sum += difference * difference;
    }

    if (trials_ > 1)
    {
        standard_deviation_ = std::sqrt(squared_sum / static_cast<double>(trials_ - 1));
    }
    else
    {
        standard_deviation_ = 0.0;
    }

    const double confidence_delta =
        CONFIDENCE_95_COEFFICIENT * standard_deviation_
        / std::sqrt(static_cast<double>(trials_));

    confidence_low_ = mean_ - confidence_delta;
    confidence_high_ = mean_ + confidence_delta;
}
