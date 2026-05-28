#pragma once

#include <cstddef>
#include <vector>

struct PercolationStats
{
    PercolationStats(std::size_t dimension, std::size_t trials);

    double get_mean() const;
    double get_standard_deviation() const;
    double get_confidence_low() const;
    double get_confidence_high() const;
    void execute();

private:
    std::size_t m_dimension;
    std::size_t m_trials;
    std::vector<double> m_results;
    double m_mean;
    double m_standard_deviation;
    double m_confidence_low;
    double m_confidence_high;
};
