#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "PercolationStats.h"

int main()
{
    try
    {
        size_t dimension;
        size_t trials;

        std::cout << "Enter dimension and trials: ";
        std::cin >> dimension >> trials;

        PercolationStats stats(dimension, trials);
        stats.execute();

        std::cout << std::fixed << std::setprecision(6);

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
    catch (const std::exception& exception)
    {
        std::cerr << "error: " << exception.what() << '\n';
        return 1;
    }

    return 0;
}