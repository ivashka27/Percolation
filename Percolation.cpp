#include "Percolation.h"

#include <stdexcept>

Percolation::Percolation(size_t n)
    : dimension(n),
    opened(n* n, 0),
    opened_count(0),
    union_find(n* n + 2),
    union_find_for_fullness(n* n + 1),
    virtual_top(n* n),
    virtual_bottom(n* n + 1)
{
    if (n == 0)
    {
        throw std::invalid_argument("Dimension must be greater than zero");
    }
}

size_t Percolation::get_index(size_t row, size_t col) const
{
    return row * dimension + col;
}

void Percolation::validate(size_t row, size_t col) const
{
    if (row >= dimension || col >= dimension)
    {
        throw std::out_of_range("Cell index is out of range");
    }
}

void Percolation::open(size_t row, size_t col)
{
    validate(row, col);

    if (is_open(row, col))
    {
        return;
    }

    size_t current_index = get_index(row, col);

    opened[current_index] = 1;
    ++opened_count;

    if (row == 0)
    {
        union_find.unite(current_index, virtual_top);
        union_find_for_fullness.unite(current_index, virtual_top);
    }

    if (row == dimension - 1)
    {
        union_find.unite(current_index, virtual_bottom);
    }

    const int d_row[4] = { -1, 1, 0, 0 };
    const int d_col[4] = { 0, 0, -1, 1 };

    for (int i = 0; i < 4; ++i)
    {
        int new_row = static_cast<int>(row) + d_row[i];
        int new_col = static_cast<int>(col) + d_col[i];

        if (new_row < 0 || new_row >= static_cast<int>(dimension))
        {
            continue;
        }

        if (new_col < 0 || new_col >= static_cast<int>(dimension))
        {
            continue;
        }

        size_t neighbor_row = static_cast<size_t>(new_row);
        size_t neighbor_col = static_cast<size_t>(new_col);

        if (is_open(neighbor_row, neighbor_col))
        {
            size_t neighbor_index = get_index(neighbor_row, neighbor_col);

            union_find.unite(current_index, neighbor_index);
            union_find_for_fullness.unite(current_index, neighbor_index);
        }
    }
}

bool Percolation::is_open(size_t row, size_t col) const
{
    validate(row, col);

    size_t index = get_index(row, col);
    return opened[index] == 1;
}

bool Percolation::is_full(size_t row, size_t col)
{
    validate(row, col);

    if (!is_open(row, col))
    {
        return false;
    }

    size_t index = get_index(row, col);
    return union_find_for_fullness.connected(index, virtual_top);
}

bool Percolation::percolates()
{
    return union_find.connected(virtual_top, virtual_bottom);
}

size_t Percolation::get_open_count() const
{
    return opened_count;
}

size_t Percolation::get_dimension() const
{
    return dimension;
}