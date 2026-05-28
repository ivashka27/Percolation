#include <iostream>
#include <iomanip>
#include "percolation_stats.h"

int main() {
    // Classic Princeton parameters: 200×200 grid, 100 trials
    size_t n      = 200;
    size_t trials = 100;

    PercolationStats stats(n, trials);
    stats.execute();

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Grid size         : " << n << "x" << n << "\n";
    std::cout << "Trials            : " << trials << "\n";
    std::cout << "Mean              : " << stats.get_mean() << "\n";
    std::cout << "Std deviation     : " << stats.get_standard_deviation() << "\n";
    std::cout << "95% confidence    : ["
              << stats.get_confidence_low() << ", "
              << stats.get_confidence_high() << "]\n";
    // Expected: threshold ≈ 0.593
    return 0;
}
