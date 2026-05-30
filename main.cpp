#include <iostream>
#include <iomanip>
#include "percolation_stats.h"

int main() {
    size_t dimension = 100; 
    size_t trials = 100;    

    std::cout << "Calculating percolation threshold for " << dimension << "x" << dimension
        << " grid with " << trials << " trials..." << std::endl;

    PercolationStats stats(dimension, trials);
    stats.execute();

    std::cout << std::fixed << std::setprecision(6);
    std::cout << "Mean percolation threshold = " << stats.get_mean() << std::endl;
    std::cout << "Standard deviation       = " << stats.get_standard_deviation() << std::endl;
    std::cout << "95% Confidence Interval  = [" << stats.get_confidence_low() << ", " << stats.get_confidence_high() << "]" << std::endl;

    return 0;
}
