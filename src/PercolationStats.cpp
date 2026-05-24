#include "PercolationStats.hpp"
#include "Percolation.hpp"
#include <random>
#include <cmath>

PercolationStats::PercolationStats(size_t dimension, size_t trials):
    m_dimension(dimension),
    m_trials(trials),
    m_thresholds(trials, 0.0),
    m_mean(0.0), 
    m_stddev(0.0), 
    m_conf_low(0.0), 
    m_conf_high(0.0), 
    m_executed(false)
{
}

void PercolationStats::execute()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, m_dimension - 1);

    for (size_t t = 0; t < m_trials; ++t)
    {
        Percolation perc(m_dimension);
        while (!perc.percolates())
        {
            size_t row = dist(gen);
            size_t col = dist(gen);
            if (!perc.is_open(row, col))
            {
                perc.open(row, col);
            }
        }
        m_thresholds[t] = static_cast<double>(perc.get_open_count()) / (m_dimension * m_dimension);
    }

    double sum = 0.0;
    for (size_t t = 0; t < m_trials; ++t)
    {
        sum += m_thresholds[t];
    }
    m_mean = sum / m_trials;

    double sum_sq = 0.0;
    for (size_t t = 0; t < m_trials; ++t)
    {
        double diff = m_thresholds[t] - m_mean;
        sum_sq += diff * diff;
    }
    m_stddev = std::sqrt(sum_sq / (m_trials - 1));

    double confidence_interval = 1.96 * m_stddev / std::sqrt(m_trials);
    m_conf_low = m_mean - confidence_interval;
    m_conf_high = m_mean + confidence_interval;

    m_executed = true;
}

double PercolationStats::get_mean() const
{
    return m_mean;
}

double PercolationStats::get_standard_deviation() const
{
    return m_stddev;
}

double PercolationStats::get_confidence_low() const
{
    return m_conf_low;
}

double PercolationStats::get_confidence_high() const
{
    return m_conf_high;
}