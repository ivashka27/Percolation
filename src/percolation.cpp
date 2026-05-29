#include "percolation.hpp"

#include <stdexcept>

DisjointSet::DisjointSet(std::size_t size)
    : parent(size), tree_size(size, 1)
{
    for (std::size_t i = 0; i < size; ++i)
    {
        parent[i] = i;
    }
}

std::size_t DisjointSet::find(std::size_t value)
{
    while (value != parent[value])
    {
        parent[value] = parent[parent[value]];
        value = parent[value];
    }

    return value;
}

void DisjointSet::unite(std::size_t first, std::size_t second)
{
    std::size_t first_root = find(first);
    std::size_t second_root = find(second);

    if (first_root == second_root)
    {
        return;
    }

    if (tree_size[first_root] < tree_size[second_root])
    {
        parent[first_root] = second_root;
        tree_size[second_root] += tree_size[first_root];
    }
    else
    {
        parent[second_root] = first_root;
        tree_size[first_root] += tree_size[second_root];
    }
}

bool DisjointSet::connected(std::size_t first, std::size_t second)
{
    return find(first) == find(second);
}

bool DisjointSet::connected(std::size_t first, std::size_t second) const
{
    return find_const(first) == find_const(second);
}

std::size_t DisjointSet::find_const(std::size_t value) const
{
    while (value != parent[value])
    {
        value = parent[value];
    }

    return value;
}

Percolation::Percolation(std::size_t dimension)
    : grid_size(dimension),
      open_count(0),
      top_index(dimension * dimension),
      bottom_index(dimension * dimension + 1),
      opened(dimension * dimension, false),
      percolation_set(dimension * dimension + 2),
      fullness_set(dimension * dimension + 1)
{
    if (dimension == 0)
    {
        throw std::invalid_argument("dimension must be positive");
    }
}

void Percolation::open(std::size_t row, std::size_t column)
{
    check_cell(row, column);

    std::size_t current_index = index(row, column);
    if (opened[current_index])
    {
        return;
    }

    opened[current_index] = true;
    ++open_count;

    if (row == 0)
    {
        percolation_set.unite(current_index, top_index);
        fullness_set.unite(current_index, top_index);
    }

    if (row + 1 == grid_size)
    {
        percolation_set.unite(current_index, bottom_index);
    }

    if (row > 0)
    {
        connect_if_open(row - 1, column, current_index);
    }

    if (row + 1 < grid_size)
    {
        connect_if_open(row + 1, column, current_index);
    }

    if (column > 0)
    {
        connect_if_open(row, column - 1, current_index);
    }

    if (column + 1 < grid_size)
    {
        connect_if_open(row, column + 1, current_index);
    }
}

bool Percolation::is_open(std::size_t row, std::size_t column) const
{
    check_cell(row, column);
    return opened[index(row, column)];
}

bool Percolation::is_full(std::size_t row, std::size_t column) const
{
    check_cell(row, column);

    std::size_t current_index = index(row, column);
    return opened[current_index] && fullness_set.connected(current_index, top_index);
}

bool Percolation::has_percolation() const
{
    return percolation_set.connected(top_index, bottom_index);
}

bool Percolation::percolates() const
{
    return has_percolation();
}

std::size_t Percolation::get_dimension() const
{
    return grid_size;
}

std::size_t Percolation::get_open_count() const
{
    return open_count;
}

std::size_t Percolation::index(std::size_t row, std::size_t column) const
{
    return row * grid_size + column;
}

void Percolation::check_cell(std::size_t row, std::size_t column) const
{
    if (row >= grid_size || column >= grid_size)
    {
        throw std::out_of_range("cell is out of grid");
    }
}

void Percolation::connect_if_open(std::size_t row, std::size_t column, std::size_t current_index)
{
    std::size_t next_index = index(row, column);
    if (!opened[next_index])
    {
        return;
    }

    percolation_set.unite(current_index, next_index);
    fullness_set.unite(current_index, next_index);
}
