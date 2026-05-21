#include "../inc/PercolationStats.h"
#include "../inc/Percolation.h"

#include <random>
#include <algorithm>
#include <cmath>

PercolationStats::PercolationStats(size_t dimension, size_t trials)
    : dimension(dimension),
        trials(trials),
        mean(0.0),
        standardDeviation(0.0),
        confidenceLow(0.0),
        confidenceHigh(0.0) {}

void PercolationStats::execute() 
{
    results.clear();

    std::random_device rd;
    std::mt19937 generator(rd());

    size_t totalCells = dimension * dimension;

    for (size_t t = 0; t < trials; ++t) 
    {
        Percolation percolation(dimension);

        std::vector<size_t> cells(totalCells);

        for (size_t i = 0; i < totalCells; ++i) 
        {
            cells[i] = i;
        }

        std::shuffle(cells.begin(), cells.end(), generator);

        size_t currentCell = 0;

        while (!percolation.percolates()) 
        {
            size_t cell = cells[currentCell++];

            size_t row = cell / dimension;
            size_t col = cell % dimension;

            percolation.open(row, col);
        }

        double threshold =
            static_cast<double>(percolation.get_opened_count()) /
            static_cast<double>(totalCells);

        results.push_back(threshold);
    }

    mean = 0.0;

    for (double value : results) 
    {
        mean += value;
    }

    mean /= static_cast<double>(trials);

    standardDeviation = 0.0;

    for (double value : results) 
    {
        standardDeviation += (value - mean) * (value - mean);
    }

    if (trials > 1) 
    {
        standardDeviation = std::sqrt(standardDeviation / (trials - 1));
    } 
    else 
    {
        standardDeviation = 0.0;
    }

    double delta = 1.96 * standardDeviation / std::sqrt(static_cast<double>(trials));

    confidenceLow = mean - delta;
    confidenceHigh = mean + delta;
}

double PercolationStats::get_mean() const 
{
    return mean;
}

double PercolationStats::get_standard_deviation() const 
{
    return standardDeviation;
}

double PercolationStats::get_confidence_low() const 
{
    return confidenceLow;
}

double PercolationStats::get_confidence_high() const 
{
    return confidenceHigh;
}