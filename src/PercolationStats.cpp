#include "PercolationStats.h"
#include "Percolation.h"

#include <random>
#include <cmath>
#include <algorithm>

PercolationStats::PercolationStats(size_t dimension, size_t trials)
    : dimension(dimension),
      trials(trials),
      mean(0.0),
      deviation(0.0)
{
}

void PercolationStats::execute()
{
    results.clear();

    std::random_device rd;
    std::mt19937 gen(rd());

    for (size_t t = 0; t < trials; ++t)
    {
        Percolation percolation(dimension);

        std::vector<std::pair<size_t, size_t>> cells;

        for (size_t i = 0; i < dimension; ++i)
        {
            for (size_t j = 0; j < dimension; ++j)
            {
                cells.push_back({i, j});
            }
        }

        std::shuffle(cells.begin(), cells.end(), gen);

        size_t index = 0;

        while (!percolation.percolates())
        {
            percolation.open(cells[index].first, cells[index].second);
            ++index;
        }

        double threshold =
            static_cast<double>(percolation.number_of_open_sites()) /
            static_cast<double>(dimension * dimension);

        results.push_back(threshold);
    }

    mean = 0.0;

    for (double value : results)
    {
        mean += value;
    }

    mean /= static_cast<double>(trials);

    deviation = 0.0;

    for (double value : results)
    {
        deviation += (value - mean) * (value - mean);
    }

    deviation /= static_cast<double>(trials - 1);

    deviation = std::sqrt(deviation);
}

double PercolationStats::get_mean() const
{
    return mean;
}

double PercolationStats::get_standard_deviation() const
{
    return deviation;
}

double PercolationStats::get_confidence_low() const
{
    return mean - (1.96 * deviation / std::sqrt(static_cast<double>(trials)));
}

double PercolationStats::get_confidence_high() const
{
    return mean + (1.96 * deviation / std::sqrt(static_cast<double>(trials)));
}