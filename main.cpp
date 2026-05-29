#include "percolation.h"
#include "percolation_stats.h"
#include <iostream>

int main() {
    Percolation p(3);
    p.open(0, 1);
    p.open(1, 1);
    p.open(2, 1);
    std::cout << "Percolates test: ";
    if (p.percolates()) {
        std::cout << "OK\n";
    } else {
        std::cout << "FAILED\n";
    }
    PercolationStats stats(100, 100);
    stats.execute();
    std::cout << "mean = " << stats.get_mean() << '\n';
    std::cout << "stddev = " << stats.get_standard_deviation() << '\n';
    std::cout << "95% confidence interval = ["
              << stats.get_confidence_low() << ", "
              << stats.get_confidence_high() << "]\n";
    return 0;
}