#include "percolation_stats.hpp"

#include "percolation.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>
#include <stdexcept>

PercolationStats::PercolationStats(std::size_t dimension, std::size_t trials)
    : grid_dimension(dimension),
      experiment_count(trials),
      mean_value(0.0),
      standard_deviation(0.0),
      confidence_low(0.0),
      confidence_high(0.0)
{
    if (dimension == 0 || trials == 0)
    {
        throw std::invalid_argument("dimension and trials must be positive");
    }
}

double PercolationStats::get_mean() const
{
    return mean_value;
}

double PercolationStats::get_standard_deviation() const
{
    return standard_deviation;
}

double PercolationStats::get_confidence_low() const
{
    return confidence_low;
}

double PercolationStats::get_confidence_high() const
{
    return confidence_high;
}

void PercolationStats::execute()
{
    thresholds.clear();
    thresholds.reserve(experiment_count);

    for (std::size_t i = 0; i < experiment_count; ++i)
    {
        thresholds.push_back(run_one_trial());
    }

    calculate_values();
}

double PercolationStats::run_one_trial()
{
    Percolation percolation(grid_dimension);

    std::vector<std::size_t> cells(grid_dimension * grid_dimension);
    std::iota(cells.begin(), cells.end(), 0);

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::shuffle(cells.begin(), cells.end(), generator);

    std::size_t current_cell = 0;
    while (!percolation.has_percolation())
    {
        std::size_t cell = cells[current_cell];
        std::size_t row = cell / grid_dimension;
        std::size_t column = cell % grid_dimension;

        percolation.open(row, column);
        ++current_cell;
    }

    double all_cells = static_cast<double>(grid_dimension * grid_dimension);
    return static_cast<double>(percolation.get_open_count()) / all_cells;
}

void PercolationStats::calculate_values()
{
    double sum = std::accumulate(thresholds.begin(), thresholds.end(), 0.0);
    mean_value = sum / static_cast<double>(experiment_count);

    if (experiment_count == 1)
    {
        standard_deviation = 0.0;
    }
    else
    {
        double squared_sum = 0.0;
        for (double threshold : thresholds)
        {
            double diff = threshold - mean_value;
            squared_sum += diff * diff;
        }

        standard_deviation = std::sqrt(squared_sum / static_cast<double>(experiment_count - 1));
    }

    double half_interval = 1.96 * standard_deviation / std::sqrt(static_cast<double>(experiment_count));
    confidence_low = mean_value - half_interval;
    confidence_high = mean_value + half_interval;
}
