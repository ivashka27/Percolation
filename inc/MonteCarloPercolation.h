#pragma once
#include <vector>

class MonteCarloPercolation {
private:
    int dimension;
    int experiment_count;
    std::vector<double> thresholds;
    double avg;
    double std_dev;
    double ci_low;
    double ci_high;

public:
    MonteCarloPercolation(int dim, int trials);

    void run_experiments();

    double get_average() const;
    double get_std_deviation() const;
    double get_ci_lower() const;
    double get_ci_upper() const;
};
