#include "PercolationStats.h"
#include "Percolation.h"

#include <cmath>
#include <random>
#include <vector>

using namespace std;

PercolationStats::PercolationStats(size_t dimension, size_t trials)
{
    this->dimension = dimension;
    this->trials = trials;
    mean = 0;
    deviation = 0;
    conf_low = 0;
    conf_high = 0;
}

void PercolationStats::execute()
{
    // we do the experiment T times and remember every result here
    vector<double> results(trials);

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<size_t> dist(0, dimension - 1);

    for (size_t t = 0; t < trials; t++) {
        Percolation perc(dimension);
        // open random cells until the system start to percolate
        while (!perc.is_percolating()) {
            size_t row = dist(gen);
            size_t col = dist(gen);
            // if the cell is already open we just pick another one next time
            if (!perc.is_open(row, col)) {
                perc.open(row, col);
            }
        }
        double p = (double) perc.number_of_opened() / (dimension * dimension);
        results[t] = p;
    }

    // mean value x-
    double sum = 0;
    for (size_t i = 0; i < trials; i++) {
        sum += results[i];
    }
    mean = sum / trials;

    // this is the unbiased variance s^2 like in the formula
    double sq = 0;
    for (size_t i = 0; i < trials; i++) {
        sq += (results[i] - mean) * (results[i] - mean);
    }
    double variance = sq / (trials - 1);
    deviation = sqrt(variance);

    // 1.96 is the magic number for the 95 percent interval
    double delta = 1.96 * deviation / sqrt((double) trials);
    conf_low = mean - delta;
    conf_high = mean + delta;
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
    return conf_low;
}

double PercolationStats::get_confidence_high() const
{
    return conf_high;
}
