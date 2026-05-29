#include "PercolationStats.h"
#include <iostream>

int main() {
    size_t dimension = 100;
    size_t trials = 100;

    PercolationStats stats(dimension, trials);
    stats.execute();

    std::cout << "Mean                    = " << stats.get_mean() << "\n";
    std::cout << "Standard Deviation      = " << stats.get_standard_deviation() << "\n";
    std::cout << "Confidence Interval Low  = " << stats.get_confidence_low() << "\n";
    std::cout << "Confidence Interval High = " << stats.get_confidence_high() << "\n";

    return 0;
}