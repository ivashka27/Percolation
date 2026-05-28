#include "percolation_stats.h"

#include "percolation.h"

#include <cmath>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>

PercolationStats::PercolationStats(std::size_t dimension, std::size_t trials)
    : m_dimension(dimension),
      m_trials(trials),
      m_results(),
      m_mean(0.0),
      m_standard_deviation(0.0),
      m_confidence_low(0.0),
      m_confidence_high(0.0)
{
    if (dimension == 0)
    {
        throw std::invalid_argument("dimension must be greater than zero");
    }

    if (trials == 0)
    {
        throw std::invalid_argument("trials must be greater than zero");
    }
}

double PercolationStats::get_mean() const
{
    return m_mean;
}

double PercolationStats::get_standard_deviation() const
{
    return m_standard_deviation;
}

double PercolationStats::get_confidence_low() const
{
    return m_confidence_low;
}

double PercolationStats::get_confidence_high() const
{
    return m_confidence_high;
}

void PercolationStats::execute()
{
    m_results.clear();
    m_results.reserve(m_trials);

    std::random_device random_device;
    std::mt19937 generator(random_device());

    for (std::size_t trial = 0; trial < m_trials; ++trial)
    {
        Percolation percolation(m_dimension);
        std::vector<std::pair<std::size_t, std::size_t>> blocked_cells;
        blocked_cells.reserve(m_dimension * m_dimension);

        for (std::size_t row = 0; row < m_dimension; ++row)
        {
            for (std::size_t column = 0; column < m_dimension; ++column)
            {
                blocked_cells.push_back({row, column});
            }
        }

        while (!percolation.percolates())
        {
            std::uniform_int_distribution<std::size_t> distribution(0, blocked_cells.size() - 1);
            std::size_t random_index = distribution(generator);
            std::pair<std::size_t, std::size_t> cell = blocked_cells[random_index];

            percolation.open(cell.first, cell.second);

            blocked_cells[random_index] = blocked_cells.back();
            blocked_cells.pop_back();
        }

        double total_cells = static_cast<double>(m_dimension * m_dimension);
        double opened_cells = static_cast<double>(percolation.get_open_sites());
        m_results.push_back(opened_cells / total_cells);
    }

    double sum = 0.0;
    for (double value : m_results)
    {
        sum += value;
    }
    m_mean = sum / static_cast<double>(m_results.size());

    if (m_results.size() == 1)
    {
        m_standard_deviation = 0.0;
        m_confidence_low = m_mean;
        m_confidence_high = m_mean;
        return;
    }

    double square_sum = 0.0;
    for (double value : m_results)
    {
        double diff = value - m_mean;
        square_sum += diff * diff;
    }

    m_standard_deviation = std::sqrt(square_sum / static_cast<double>(m_results.size() - 1));

    double delta = 1.96 * m_standard_deviation / std::sqrt(static_cast<double>(m_results.size()));
    m_confidence_low = m_mean - delta;
    m_confidence_high = m_mean + delta;
}
