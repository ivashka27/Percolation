#include "Percolation.h"

Percolation::Percolation(size_t n)
    : n(n), grid(n, std::vector<bool>(n, false))
{
}

void Percolation::open(size_t row, size_t col)
{
    grid[row][col] = true;
}

bool Percolation::is_open(size_t row, size_t col) const
{
    return grid[row][col];
}

size_t Percolation::number_of_open_sites() const
{
    size_t count = 0;

    for (size_t i = 0; i < n; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            if (grid[i][j])
            {
                ++count;
            }
        }
    }

    return count;
}

bool Percolation::dfs(size_t row, size_t col, std::vector<std::vector<bool>>& used) const
{
    if (row >= n || col >= n)
    {
        return false;
    }

    if (!grid[row][col] || used[row][col])
    {
        return false;
    }

    used[row][col] = true;

    if (row == n - 1)
    {
        return true;
    }

    if (row > 0 && dfs(row - 1, col, used))
    {
        return true;
    }

    if (row + 1 < n && dfs(row + 1, col, used))
    {
        return true;
    }

    if (col > 0 && dfs(row, col - 1, used))
    {
        return true;
    }

    if (col + 1 < n && dfs(row, col + 1, used))
    {
        return true;
    }

    return false;
}

bool Percolation::percolates() const
{
    std::vector<std::vector<bool>> used(n, std::vector<bool>(n, false));

    for (size_t col = 0; col < n; ++col)
    {
        if (grid[0][col])
        {
            if (dfs(0, col, used))
            {
                return true;
            }
        }
    }

    return false;
}