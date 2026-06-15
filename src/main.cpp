#include "PercolationStats.hpp"
#include <iostream>

int main() {
    std::size_t n = 200;
    std::size_t trials = 100;

    PercolationStats stats(n, trials);
    stats.execute();

    std::cout << "Mean                    = " << stats.get_mean() << "\n";
    std::cout << "Standard deviation      = " << stats.get_standard_deviation() << "\n";
    std::cout << "95% confidence interval = [" 
              << stats.get_confidence_low() << ", " 
              << stats.get_confidence_high() << "]\n";

    return 0;
}