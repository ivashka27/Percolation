#include "PercolationStats.h"
#include "Percolation.h"

#include <random>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <stdexcept>

PercolationStats::PercolationStats(size_t dimension, size_t trials)
    : dimension(dimension),
    trials(trials),
    mean(0.0),
    standard_deviation(0.0),
    confidence_low(0.0),
    confidence_high(0.0),
    executed(false)
{
    if (dimension == 0)
    {
        throw std::invalid_argument("Dimension must be greater than zero");
    }

    if (trials == 0)
    {
        throw std::invalid_argument("Trials must be greater than zero");
    }
}

void PercolationStats::execute()
{
    results.clear();
    results.reserve(trials);

    std::random_device random_device;
    std::mt19937 generator(random_device());

    const size_t total_cells = dimension * dimension;

    for (size_t trial = 0; trial < trials; ++trial)
    {
        Percolation percolation(dimension);

        std::vector<size_t> cells(total_cells);

        for (size_t i = 0; i < total_cells; ++i)
        {
            cells[i] = i;
        }

        std::shuffle(cells.begin(), cells.end(), generator);

        size_t current_cell = 0;

        while (!percolation.percolates())
        {
            size_t cell_index = cells[current_cell];

            size_t row = cell_index / dimension;
            size_t col = cell_index % dimension;

            percolation.open(row, col);

            ++current_cell;
        }

        double threshold =
            static_cast<double>(percolation.get_open_count()) /
            static_cast<double>(total_cells);

        results.push_back(threshold);
    }

    calculate_statistics();
    executed = true;
}

void PercolationStats::calculate_statistics()
{
    double sum = 0.0;

    for (double value : results)
    {
        sum += value;
    }

    mean = sum / static_cast<double>(trials);

    if (trials == 1)
    {
        standard_deviation = 0.0;
    }
    else
    {
        double squared_sum = 0.0;

        for (double value : results)
        {
            double difference = value - mean;
            squared_sum += difference * difference;
        }

        standard_deviation = std::sqrt(
            squared_sum / static_cast<double>(trials - 1)
        );
    }

    double margin =
        1.96 * standard_deviation / std::sqrt(static_cast<double>(trials));

    confidence_low = mean - margin;
    confidence_high = mean + margin;
}

double PercolationStats::get_mean() const
{
    return mean;
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