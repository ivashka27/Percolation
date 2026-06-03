#include "Percolation.h"
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <iostream>

struct PercolationAnalytics {
private:
    size_t side_size;
    size_t total_trials;
    double mean_value;
    double deviation_value;
    double interval_low;
    double interval_high;
    std::vector<double> history_metrics;

public:
    PercolationAnalytics(size_t dimension, size_t trials)
        : side_size(dimension), 
          total_trials(trials), 
          mean_value(0.0), 
          deviation_value(0.0), 
          interval_low(0.0), 
          interval_high(0.0) 
    {
        history_metrics.resize(trials, 0.0);
    }

    double get_mean() const { return mean_value; }
    double get_deviation() const { return deviation_value; }
    double get_low_bound() const { return interval_low; }
    double get_high_bound() const { return interval_high; }

    void run_analysis() {
        if (total_trials == 0 || side_size == 0) return;

        std::random_device rand_dev;
        std::mt19937 generator(rand_dev());

        std::vector<std::pair<size_t, size_t>> cell_pool;
        cell_pool.reserve(side_size * side_size);
        for (size_t r = 0; r < side_size; ++r) {
            for (size_t c = 0; c < side_size; ++c) {
                cell_pool.push_back({r, c});
            }
        }

        for (size_t t = 0; t < total_trials; ++t) {
            Percolation sys(side_size);
            
            std::shuffle(cell_pool.begin(), cell_pool.end(), generator);

            for (const auto& cell : cell_pool) {
                sys.open_cell(cell.first, cell.second);
                if (sys.system_percolates()) {
                    break;
                }
            }

            double max_cells = static_cast<double>(side_size * side_size);
            history_metrics[t] = static_cast<double>(sys.get_active_cells_count()) / max_cells;
        }

        double total_sum = 0.0;
        for (double val : history_metrics) {
            total_sum += val;
        }
        mean_value = total_sum / total_trials;

        if (total_trials > 1) {
            double variance_accum = 0.0;
            for (double val : history_metrics) {
                variance_accum += (val - mean_value) * (val - mean_value);
            }
            double variance = variance_accum / (total_trials - 1);
            deviation_value = std::sqrt(variance);

            double error_margin = (1.96 * deviation_value) / std::sqrt(total_trials);
            interval_low = mean_value - error_margin;
            interval_high = mean_value + error_margin;
        }
    }
};

int main() {
    size_t grid_dim = 100;
    size_t test_runs = 100;

    PercolationAnalytics analytics(grid_dim, test_runs);
    analytics.run_analysis();

    std::cout << "Mean                     = " << analytics.get_mean() << "\n";
    std::cout << "Standard Deviation       = " << analytics.get_deviation() << "\n";
    std::cout << "Confidence Interval Low  = " << analytics.get_low_bound() << "\n";
    std::cout << "Confidence Interval High = " << analytics.get_high_bound() << "\n";

    return 0;
}