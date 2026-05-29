#include "Percolatioin.hpp"
#include "PercolationStats.hpp"

#include <iostream>

int main() {
  Percolation percolation(3);

  std::cout << "Grid  : " << percolation.getNumberOfCellsInRow() << "x"
            << percolation.getNumberOfCellsInRow() << '\n';
  std::cout << "Open cells: " << percolation.open_cells_count() << '\n';

  percolation.open(0, 1);
  percolation.open(1, 1);
  percolation.open(2, 1);

  std::cout << percolation.isOpen(1, 1) << '\n';
  std::cout << "Percolates: " << percolation.percolates() << '\n';

  PercolationStats stats(3, 5);
  stats.execute();

  std::cout << "Mean threshold: " << stats.get_mean() << '\n';
  std::cout << "Stddev: " << stats.get_standard_deviation() << '\n';
  std::cout << "Confidence low: " << stats.get_confidence_low() << '\n';
  std::cout << "Confidence high: " << stats.get_confidence_high() << '\n';

  return 0;
}
