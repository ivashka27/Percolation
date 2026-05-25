#include "percolation_stats.hpp"
#include <iostream>
#include <iomanip>

int main() {
    {
        PercolationStats stats(20, 100);
        stats.execute();
        
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "=== 20x20, 100 trials ===\n";
        std::cout << "mean = " << stats.get_mean() << "\n";
        std::cout << "stddev = " << stats.get_standard_deviation() << "\n";
        std::cout << "95% confidence interval = [" << stats.get_confidence_low() << ", " << stats.get_confidence_high() << "]\n\n";
    }

    {
        PercolationStats stats(100, 200);
        stats.execute();
        
        std::cout << "=== 100x100, 200 trials ===\n";
        std::cout << "mean = " << stats.get_mean() << "\n";
        std::cout << "stddev = " << stats.get_standard_deviation() << "\n";
        std::cout << "95% confidence interval = [" << stats.get_confidence_low() << ", " << stats.get_confidence_high() << "]\n\n";
    }

    {
        Percolation p(5);
        for (size_t i = 0; i < 5; ++i) p.open_cell(i, 0);
        
        std::cout << "=== Manual 5x5 test (column 0 open) ===\n";
        std::cout << "percolates: " << (p.percolates() ? "YES" : "NO") << "\n";
        std::cout << "cell (4,0) is full: " << (p.is_full(4, 0) ? "YES" : "NO") << "\n";
        std::cout << "cell (4,4) is full: " << (p.is_full(4, 4) ? "YES" : "NO") << "\n";
    }

    return 0;
}