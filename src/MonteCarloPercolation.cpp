#include "../inc/MonteCarloPercolation.h"
#include "../inc/PercolationModel.h"
#include <random>
#include <algorithm>
#include <cmath>

MonteCarloPercolation::MonteCarloPercolation(int dim, int trials)
    : dimension(dim)
    , experiment_count(trials)
    , avg(0.0)
    , std_dev(0.0)
    , ci_low(0.0)
    , ci_high(0.0) {}

void MonteCarloPercolation::run_experiments() {
    thresholds.clear();
    thresholds.reserve(experiment_count);

    std::random_device rd;
    std::mt19937 rng(rd());

    int total_cells = dimension * dimension;

    for (int exp = 0; exp < experiment_count; ++exp) {
        PercolationModel model(dimension);

        std::vector<int> indices(total_cells);
        for (int i = 0; i < total_cells; ++i) {
            indices[i] = i;
        }
        std::shuffle(indices.begin(), indices.end(), rng);

        int step = 0;
        while (!model.does_percolate()) {
            int cell = indices[step++];
            int row = cell / dimension;
            int col = cell % dimension;
            model.activate(row, col);
        }

        double fraction = static_cast<double>(model.get_number_open()) / static_cast<double>(total_cells);
        thresholds.push_back(fraction);
    }

    avg = 0.0;
    for (double val : thresholds) {
        avg += val;
    }
    avg /= static_cast<double>(experiment_count);

    std_dev = 0.0;
    for (double val : thresholds) {
        double diff = val - avg;
        std_dev += diff * diff;
    }
    if (experiment_count > 1) {
        std_dev = std::sqrt(std_dev / static_cast<double>(experiment_count - 1));
    } else {
        std_dev = 0.0;
    }

    double margin = 1.96 * std_dev / std::sqrt(static_cast<double>(experiment_count));
    ci_low = avg - margin;
    ci_high = avg + margin;
}

double MonteCarloPercolation::get_average() const {
    return avg;
}

double MonteCarloPercolation::get_std_deviation() const {
    return std_dev;
}

double MonteCarloPercolation::get_ci_lower() const {
    return ci_low;
}

double MonteCarloPercolation::get_ci_upper() const {
    return ci_high;
}
