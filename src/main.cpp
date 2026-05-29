#include "percolation_stats.hpp"

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
    try
    {
        std::size_t dimension = 20;
        std::size_t trials = 30;

        if (argc >= 2)
        {
            dimension = std::stoul(argv[1]);
        }

        if (argc >= 3)
        {
            trials = std::stoul(argv[2]);
        }

        PercolationStats stats(dimension, trials);
        stats.execute();

        std::cout << std::fixed << std::setprecision(6);
        std::cout << "mean = " << stats.get_mean() << '\n';
        std::cout << "stddev = " << stats.get_standard_deviation() << '\n';
        std::cout << "confidence low = " << stats.get_confidence_low() << '\n';
        std::cout << "confidence high = " << stats.get_confidence_high() << '\n';
    }
    catch (const std::exception& error)
    {
        std::cerr << "error " << error.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
