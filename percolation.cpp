#include "percolation.h"

#include <queue>
#include <stdexcept>
#include <utility>
#include <vector>

Percolation::Percolation(std::size_t dimension)
    : m_dimension(dimension), m_open(dimension * dimension, false), m_open_sites(0)
{
    if (dimension == 0)
    {
        throw std::invalid_argument("dimension must be greater than zero");
    }
}

void Percolation::open(std::size_t row, std::size_t column)
{
    check_coordinates(row, column);

    std::size_t cell_index = index(row, column);
    if (m_open[cell_index])
    {
        return;
    }

    m_open[cell_index] = true;
    m_open_sites += 1;
}

bool Percolation::is_open(std::size_t row, std::size_t column) const
{
    check_coordinates(row, column);
    return m_open[index(row, column)];
}

bool Percolation::is_full(std::size_t row, std::size_t column) const
{
    check_coordinates(row, column);
    if (!is_open(row, column))
    {
        return false;
    }

    std::vector<bool> used(m_dimension * m_dimension, false);
    std::queue<std::pair<std::size_t, std::size_t>> q;

    for (std::size_t current_column = 0; current_column < m_dimension; ++current_column)
    {
        if (is_open(0, current_column))
        {
            q.push({0, current_column});
            used[index(0, current_column)] = true;
        }
    }

    while (!q.empty())
    {
        std::pair<std::size_t, std::size_t> current = q.front();
        q.pop();

        if (current.first == row && current.second == column)
        {
            return true;
        }

        if (current.first > 0)
        {
            std::size_t next_row = current.first - 1;
            std::size_t next_column = current.second;
            std::size_t next_index = index(next_row, next_column);
            if (m_open[next_index] && !used[next_index])
            {
                used[next_index] = true;
                q.push({next_row, next_column});
            }
        }

        if (current.first + 1 < m_dimension)
        {
            std::size_t next_row = current.first + 1;
            std::size_t next_column = current.second;
            std::size_t next_index = index(next_row, next_column);
            if (m_open[next_index] && !used[next_index])
            {
                used[next_index] = true;
                q.push({next_row, next_column});
            }
        }

        if (current.second > 0)
        {
            std::size_t next_row = current.first;
            std::size_t next_column = current.second - 1;
            std::size_t next_index = index(next_row, next_column);
            if (m_open[next_index] && !used[next_index])
            {
                used[next_index] = true;
                q.push({next_row, next_column});
            }
        }

        if (current.second + 1 < m_dimension)
        {
            std::size_t next_row = current.first;
            std::size_t next_column = current.second + 1;
            std::size_t next_index = index(next_row, next_column);
            if (m_open[next_index] && !used[next_index])
            {
                used[next_index] = true;
                q.push({next_row, next_column});
            }
        }
    }

    return false;
}

bool Percolation::percolates() const
{
    std::vector<bool> used(m_dimension * m_dimension, false);
    std::queue<std::pair<std::size_t, std::size_t>> q;

    for (std::size_t column = 0; column < m_dimension; ++column)
    {
        if (is_open(0, column))
        {
            q.push({0, column});
            used[index(0, column)] = true;
        }
    }

    while (!q.empty())
    {
        std::pair<std::size_t, std::size_t> current = q.front();
        q.pop();

        if (current.first == m_dimension - 1)
        {
            return true;
        }

        if (current.first > 0)
        {
            std::size_t next_row = current.first - 1;
            std::size_t next_column = current.second;
            std::size_t next_index = index(next_row, next_column);
            if (m_open[next_index] && !used[next_index])
            {
                used[next_index] = true;
                q.push({next_row, next_column});
            }
        }

        if (current.first + 1 < m_dimension)
        {
            std::size_t next_row = current.first + 1;
            std::size_t next_column = current.second;
            std::size_t next_index = index(next_row, next_column);
            if (m_open[next_index] && !used[next_index])
            {
                used[next_index] = true;
                q.push({next_row, next_column});
            }
        }

        if (current.second > 0)
        {
            std::size_t next_row = current.first;
            std::size_t next_column = current.second - 1;
            std::size_t next_index = index(next_row, next_column);
            if (m_open[next_index] && !used[next_index])
            {
                used[next_index] = true;
                q.push({next_row, next_column});
            }
        }

        if (current.second + 1 < m_dimension)
        {
            std::size_t next_row = current.first;
            std::size_t next_column = current.second + 1;
            std::size_t next_index = index(next_row, next_column);
            if (m_open[next_index] && !used[next_index])
            {
                used[next_index] = true;
                q.push({next_row, next_column});
            }
        }
    }

    return false;
}

std::size_t Percolation::get_dimension() const
{
    return m_dimension;
}

std::size_t Percolation::get_open_sites() const
{
    return m_open_sites;
}

std::size_t Percolation::index(std::size_t row, std::size_t column) const
{
    return row * m_dimension + column;
}

void Percolation::check_coordinates(std::size_t row, std::size_t column) const
{
    if (row >= m_dimension || column >= m_dimension)
    {
        throw std::out_of_range("cell coordinates are out of range");
    }
}
