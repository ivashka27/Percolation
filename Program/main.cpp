#include "percolation.hpp"
#include <iostream>

int main() {
    size_t n, T;
    std::cout << "Введите размер решётки и количество экспериментов: " << "\n";
    std::cout << "Размер решётки (n): ";
    std::cin >> n;
    std::cout << "Количество экспериментов (T): ";
    std::cin >> T;

    PercolationStats stats(n, T);
    stats.execute();

    std::cout << "mean                    = " << stats.get_mean() << "\n";
    std::cout << "stddev                  = " << stats.get_standard_deviation() << "\n";
    std::cout << "95% confidence interval = ["
              << stats.get_confidence_low() << ", "
              << stats.get_confidence_high() << "]\n";

    return 0;
}