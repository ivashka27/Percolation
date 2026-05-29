#pragma once

#include <vector>
#include <cmath>
#include <numeric>
#include "percolation.h"

struct PercolationStats
{
    size_t dimension;
    size_t trials;

    double mean = 0.0;
    double stddev = 0.0;
    double confidence_low = 0.0;
    double confidence_high = 0.0;

    PercolationStats(size_t dimension,
                     size_t trials)
        : dimension(dimension),
          trials(trials)
    {
    }

    double get_mean() const
    {
        return mean;
    }

    double get_standard_deviation() const
    {
        return stddev;
    }

    double get_confidence_low() const
    {
        return confidence_low;
    }

    double get_confidence_high() const
    {
        return confidence_high;
    }

    void execute()
    {
        std::vector<double> results;

        for (size_t t = 0; t < trials; ++t)
        {
            Percolation perc(dimension);

            while (!perc.percolates())
            {
                perc.open_random_site();
            }

            double threshold =
                static_cast<double>(
                    perc.number_of_open_sites())
                / (dimension * dimension);

            results.push_back(threshold);
        }

        mean =
            std::accumulate(results.begin(),
                            results.end(),
                            0.0)
            / results.size();

        double variance = 0.0;

        for (double x : results)
        {
            variance +=
                (x - mean) * (x - mean);
        }

        variance /= (results.size() - 1);

        stddev = std::sqrt(variance);

        double delta =
            1.96 * stddev /
            std::sqrt(static_cast<double>(trials));

        confidence_low = mean - delta;
        confidence_high = mean + delta;
    }
};