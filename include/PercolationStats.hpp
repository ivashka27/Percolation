#ifndef PERCOLATION_STATS_HPP
#define PERCOLATION_STATS_HPP

#include <vector>

struct PercolationStats
{
    PercolationStats(size_t dimension, size_t trials);

    void execute();

    double get_mean() const;

    double get_standard_deviation() const;

    double get_confidence_low() const;

    double get_confidence_high() const;

private:
    size_t m_dimension;
    size_t m_trials;
    std::vector<double> m_thresholds;
    double m_mean;
    double m_stddev;
    double m_conf_low;
    double m_conf_high;
    bool m_executed;
};

#endif