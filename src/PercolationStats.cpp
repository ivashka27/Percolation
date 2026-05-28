#include "PercolationStats.h"
#include "Percolation.h"
#include <numeric>
#include <cmath>
#include <random>
#include <algorithm>


PercolationStats::PercolationStats(size_t dimension, size_t trials): 
	dimension(dimension), 
	trials(trials), 
	mean(0.0), 
	standard_deviation(0.0), 
	confidence_low(0.0), 
	confidence_high(0.0) { thresholds.resize(trials, 0.0);}


double PercolationStats::get_mean() const { return mean; }
double PercolationStats::get_standard_deviation() const { return standard_deviation; }
double PercolationStats::get_confidence_low() const { return confidence_low; }
double PercolationStats::get_confidence_high() const { return confidence_high; }


void PercolationStats::execute() {
	if (trials == 0 || dimension == 0) return;

	std::random_device rd;
	std::mt19937 gen(rd());
	std::vector<size_t> indices(dimension * dimension);
	std::iota(indices.begin(), indices.end(), 0);

	for (size_t t = 0; t < trials; ++t) {
		Percolation perc(dimension);
		std::shuffle(indices.begin(), indices.end(), gen);
		for (size_t idx : indices) {
			size_t row = idx / dimension;
			size_t col = idx % dimension;
			perc.open(row, col);
			if (perc.percolates()) { break; }
		}
		thresholds[t] = static_cast<double>(perc.get_open_sites_count()) / (dimension * dimension);
	}

	double sum = std::accumulate(thresholds.begin(), thresholds.end(), 0.0);
	mean = sum / trials;

	if (trials > 1) {
		double variance_sum = 0.0;
		for (double x : thresholds) {
			variance_sum += (x - mean) * (x - mean);
		}
		standard_deviation = std::sqrt(variance_sum / (trials - 1));
	}
	else {
		standard_deviation = 0.0;
	}

	double confidence_fraction = (1.96 * standard_deviation) / std::sqrt(trials);
	confidence_low = mean - confidence_fraction;
	confidence_high = mean + confidence_fraction;
}