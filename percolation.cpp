#include "percolation.h"

#include <cmath>
#include <numeric>
#include <queue>
#include <random>
#include <stdexcept>
#include <utility>

Percolation::Percolation(std::size_t dimension)
    : dimension(dimension), grid(dimension, std::vector<bool>(dimension, false)), open_count(0)
{
    if (dimension == 0)
    {
        throw std::invalid_argument("dimension must be positive");
    }
}

void Percolation::check_coordinates(std::size_t row, std::size_t col) const
{
    if (row >= dimension || col >= dimension)
    {
        throw std::out_of_range("cell coordinates are out of range");
    }
}

void Percolation::open(std::size_t row, std::size_t col)
{
    check_coordinates(row, col);

    if (!grid[row][col])
    {
        grid[row][col] = true;
        ++open_count;
    }
}

bool Percolation::is_open(std::size_t row, std::size_t col) const
{
    check_coordinates(row, col);
    return grid[row][col];
}

bool Percolation::is_full(std::size_t row, std::size_t col) const
{
    check_coordinates(row, col);

    if (!grid[row][col])
    {
        return false;
    }

    std::vector<std::vector<bool>> visited(dimension, std::vector<bool>(dimension, false));
    std::queue<std::pair<std::size_t, std::size_t>> cells;

    visited[row][col] = true;
    cells.push({row, col});

    while (!cells.empty())
    {
        std::size_t current_row = cells.front().first;
        std::size_t current_col = cells.front().second;
        cells.pop();

        if (current_row == 0)
        {
            return true;
        }

        if (current_row > 0 && grid[current_row - 1][current_col] && !visited[current_row - 1][current_col])
        {
            visited[current_row - 1][current_col] = true;
            cells.push({current_row - 1, current_col});
        }

        if (current_row + 1 < dimension && grid[current_row + 1][current_col] && !visited[current_row + 1][current_col])
        {
            visited[current_row + 1][current_col] = true;
            cells.push({current_row + 1, current_col});
        }

        if (current_col > 0 && grid[current_row][current_col - 1] && !visited[current_row][current_col - 1])
        {
            visited[current_row][current_col - 1] = true;
            cells.push({current_row, current_col - 1});
        }

        if (current_col + 1 < dimension && grid[current_row][current_col + 1] && !visited[current_row][current_col + 1])
        {
            visited[current_row][current_col + 1] = true;
            cells.push({current_row, current_col + 1});
        }
    }

    return false;
}

bool Percolation::percolates() const
{
    std::vector<std::vector<bool>> visited(dimension, std::vector<bool>(dimension, false));
    std::queue<std::pair<std::size_t, std::size_t>> cells;

    for (std::size_t col = 0; col < dimension; ++col)
    {
        if (grid[0][col])
        {
            visited[0][col] = true;
            cells.push({0, col});
        }
    }

    while (!cells.empty())
    {
        std::size_t current_row = cells.front().first;
        std::size_t current_col = cells.front().second;
        cells.pop();

        if (current_row == dimension - 1)
        {
            return true;
        }

        if (current_row > 0 && grid[current_row - 1][current_col] && !visited[current_row - 1][current_col])
        {
            visited[current_row - 1][current_col] = true;
            cells.push({current_row - 1, current_col});
        }

        if (current_row + 1 < dimension && grid[current_row + 1][current_col] && !visited[current_row + 1][current_col])
        {
            visited[current_row + 1][current_col] = true;
            cells.push({current_row + 1, current_col});
        }

        if (current_col > 0 && grid[current_row][current_col - 1] && !visited[current_row][current_col - 1])
        {
            visited[current_row][current_col - 1] = true;
            cells.push({current_row, current_col - 1});
        }

        if (current_col + 1 < dimension && grid[current_row][current_col + 1] && !visited[current_row][current_col + 1])
        {
            visited[current_row][current_col + 1] = true;
            cells.push({current_row, current_col + 1});
        }
    }

    return false;
}

std::size_t Percolation::get_open_count() const
{
    return open_count;
}

std::size_t Percolation::get_dimension() const
{
    return dimension;
}

PercolationStats::PercolationStats(std::size_t dimension, std::size_t trials)
    : dimension(dimension),
      trials(trials),
      thresholds(),
      mean(0.0),
      standard_deviation(0.0),
      confidence_low(0.0),
      confidence_high(0.0)
{
    if (dimension == 0)
    {
        throw std::invalid_argument("dimension must be positive");
    }

    if (trials == 0)
    {
        throw std::invalid_argument("trials must be positive");
    }
}

double PercolationStats::get_mean() const
{
    return mean;
}

double PercolationStats::get_standard_deviation() const
{
    return standard_deviation;
}

double PercolationStats::get_confidence_low() const
{
    return confidence_low;
}

double PercolationStats::get_confidence_high() const
{
    return confidence_high;
}

void PercolationStats::execute()
{
    thresholds.clear();
    thresholds.reserve(trials);

    std::random_device random_device;
    std::mt19937 generator(random_device());

    for (std::size_t trial = 0; trial < trials; ++trial)
    {
        Percolation percolation(dimension);
        std::vector<std::size_t> blocked_cells(dimension * dimension);

        for (std::size_t i = 0; i < blocked_cells.size(); ++i)
        {
            blocked_cells[i] = i;
        }

        while (!percolation.percolates())
        {
            std::uniform_int_distribution<std::size_t> distribution(0, blocked_cells.size() - 1);
            std::size_t random_index = distribution(generator);
            std::size_t cell_number = blocked_cells[random_index];

            blocked_cells[random_index] = blocked_cells.back();
            blocked_cells.pop_back();

            std::size_t row = cell_number / dimension;
            std::size_t col = cell_number % dimension;

            percolation.open(row, col);
        }

        double threshold = static_cast<double>(percolation.get_open_count()) /
                           static_cast<double>(dimension * dimension);
        thresholds.push_back(threshold);
    }

    double sum = std::accumulate(thresholds.begin(), thresholds.end(), 0.0);
    mean = sum / static_cast<double>(trials);

    if (trials == 1)
    {
        standard_deviation = 0.0;
    }
    else
    {
        double squared_sum = 0.0;

        for (std::size_t i = 0; i < thresholds.size(); ++i)
        {
            double threshold = thresholds[i];
            squared_sum += (threshold - mean) * (threshold - mean);
        }

        standard_deviation = std::sqrt(squared_sum / static_cast<double>(trials - 1));
    }

    double confidence_delta = 1.96 * standard_deviation / std::sqrt(static_cast<double>(trials));
    confidence_low = mean - confidence_delta;
    confidence_high = mean + confidence_delta;
}
