#include <iostream>
#include "PercolationStats.h"

int main()
{
    size_t n;
    size_t t;

    std::cin >> n >> t;

    PercolationStats stats(n, t);

    stats.execute();

    std::cout << "mean = " << stats.get_mean() << '\n';
    std::cout << "stddev = " << stats.get_standard_deviation() << '\n';
    std::cout << "confidence low = " << stats.get_confidence_low() << '\n';
    std::cout << "confidence high = " << stats.get_confidence_high() << '\n';

    return 0;
}