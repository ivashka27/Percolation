#ifndef PERCOLATION_STATS_H
#define PERCOLATION_STATS_H

#include <vector>
#include <cstddef>

struct PercolationStats
{
	PercolationStats(size_t dimension, size_t trials);

	double get_mean() const;
	double get_standard_deviation() const;
	double get_confidence_low() const;
	double get_confidence_high() const;

	void execute();

	private:
		size_t dimension;
		size_t trials;

		double mean;
		double standard_deviation;
		double confidence_low;
		double confidence_high;

		std::vector<double> thresholds;
};

#endif