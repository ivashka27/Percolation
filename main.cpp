#include <iostream>
#include "percolationStats.h"

int main()
{
    PercolationStats stats(100, 100);

    stats.execute();

    std::cout << "mean = "
              << stats.get_mean() << '\n';

    std::cout << "stddev = "
              << stats.get_standard_deviation() << '\n';

    std::cout << "95% confidence interval = ["
              << stats.get_confidence_low()
              << ", "
              << stats.get_confidence_high()
              << "]\n";
}