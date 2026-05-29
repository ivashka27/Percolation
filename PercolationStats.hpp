#pragma once

#include <cstddef>
#include <vector>

struct PercolationStats {
private:
  std::size_t dimension;
  std::size_t trials;
  std::vector<double> thresholds;
  double mean;
  double standardDeviation;
  double confidenceLow;
  double confidenceHigh;

public:
  PercolationStats(std::size_t dimension, std::size_t trials);

  double get_mean() const;
  double get_standard_deviation() const;
  double get_confidence_low() const;
  double get_confidence_high() const;

  void execute();
};
