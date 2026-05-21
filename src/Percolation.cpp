#include "../inc/Percolation.h"


size_t Percolation::index(size_t row, size_t col) const 
{
    return row * n + col;
}

bool Percolation::is_valid(size_t row, size_t col) const
{
    return row < n && col < n;
}

Percolation::Percolation(size_t dimension) 
    : n(dimension),
        opened(dimension * dimension, false),
        uf(dimension * dimension + 2),
        top(dimension * dimension),
        bottom(dimension * dimension + 1),
        openedCount(0) {}

void Percolation::open(size_t row, size_t col)
{
    if (!is_valid(row, col)) return;

    size_t current = index(row, col);

    if (opened[current]) return;

    opened[current] = true;
    ++openedCount;

    if (row == 0) 
    {
        uf.unite(current, top);
    }

    if (row == n - 1) 
    {
        uf.unite(current, bottom);
    }

    if (row > 0 && is_open(row - 1, col)) 
    {
        uf.unite(current, index(row - 1, col));
    }

    if (row + 1 < n && is_open(row + 1, col)) 
    {
        uf.unite(current, index(row + 1, col));
    }

    if (col > 0 && is_open(row, col - 1)) 
    {
        uf.unite(current, index(row, col - 1));
    }

    if (col + 1 < n && is_open(row, col + 1)) 
    {
        uf.unite(current, index(row, col + 1));
    }
}

bool Percolation::is_open(size_t row, size_t col) const
{
    if (!is_valid(row, col)) return false;
    return opened[index(row, col)];
}

bool Percolation::is_full(size_t row, size_t col)
{
    if (!is_open(row, col)) return false;
    return uf.connected(index(row, col), top);
}

bool Percolation::percolates()
{
    return uf.connected(top, bottom);
}

size_t Percolation::get_opened_count() const
{
    return openedCount;
}

size_t Percolation::get_size() const
{
    return n;
}