#include <iostream>
#include "Percolation.h"
#include "PercolationStats.h"

int main() {
	size_t dimension = 20;
	size_t trials = 30;

	PercolationStats stats(dimension, trials);

	stats.execute();

	std::cout << "Mean percolation threshold: " << stats.get_mean() << std::endl;
	std::cout << "Standard deviation: " << stats.get_standard_deviation() << std::endl;
	std::cout << "Confidence interval: [" << stats.get_confidence_low() << ", " << stats.get_confidence_high() << "]" << std::endl;
	
	return 0;
}