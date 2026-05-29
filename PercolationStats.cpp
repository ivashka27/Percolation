#include "PercolationStats.hpp"

#include "Percolatioin.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>

PercolationStats::PercolationStats(std::size_t dimension, std::size_t trials)
    : dimension(dimension == 0 ? 1 : dimension),
      trials(trials == 0 ? 1 : trials),
      thresholds(),
      mean(0.0),
      standardDeviation(0.0),
      confidenceLow(0.0),
      confidenceHigh(0.0) {}

double PercolationStats::get_mean() const { return mean; }

double PercolationStats::get_standard_deviation() const { return standardDeviation; }

double PercolationStats::get_confidence_low() const { return confidenceLow; }

double PercolationStats::get_confidence_high() const { return confidenceHigh; }

void PercolationStats::execute() {
  thresholds.clear();
  thresholds.reserve(trials);

  const std::size_t totalCells = dimension * dimension;
  std::vector<std::size_t> order(totalCells);
  std::iota(order.begin(), order.end(), 0);

  std::mt19937_64 rng(std::random_device{}());

  for (std::size_t trial = 0; trial < trials; ++trial) {
    Percolation percolation(dimension);
    std::shuffle(order.begin(), order.end(), rng);

    for (std::size_t index : order) {
      const std::size_t row = index / dimension;
      const std::size_t col = index % dimension;
      percolation.open(row, col);

      if (percolation.percolates()) {
        break;
      }
    }

    thresholds.push_back(static_cast<double>(percolation.open_cells_count()) / static_cast<double>(totalCells));
  }

  mean = std::accumulate(thresholds.begin(), thresholds.end(), 0.0) / static_cast<double>(thresholds.size());

  if (thresholds.size() > 1) {
    double sumSquaredDiffs = 0.0;
    for (double threshold : thresholds) {
      const double diff = threshold - mean;
      sumSquaredDiffs += diff * diff;
    }

    standardDeviation = std::sqrt(sumSquaredDiffs / static_cast<double>(thresholds.size() - 1));
  } else {
    standardDeviation = 0.0;
  }

  const double confidenceHalfWidth = 1.96 * standardDeviation / std::sqrt(static_cast<double>(trials));
  confidenceLow = mean - confidenceHalfWidth;
  confidenceHigh = mean + confidenceHalfWidth;
}
