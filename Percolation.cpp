#include "Percolation.h"

#include <numeric>
#include <stdexcept>
#include <utility>

Percolation::DisjointSetUnion::DisjointSetUnion(size_t size)
    : parent_(size), rank_(size, 0)
{
    std::iota(parent_.begin(), parent_.end(), 0);
}

size_t Percolation::DisjointSetUnion::find(size_t vertex)
{
    if (parent_[vertex] != vertex)
    {
        parent_[vertex] = find(parent_[vertex]);
    }

    return parent_[vertex];
}

void Percolation::DisjointSetUnion::unite(size_t first, size_t second)
{
    size_t first_root = find(first);
    size_t second_root = find(second);

    if (first_root == second_root)
    {
        return;
    }

    if (rank_[first_root] < rank_[second_root])
    {
        std::swap(first_root, second_root);
    }

    parent_[second_root] = first_root;

    if (rank_[first_root] == rank_[second_root])
    {
        ++rank_[first_root];
    }
}

bool Percolation::DisjointSetUnion::connected(size_t first, size_t second)
{
    return find(first) == find(second);
}

Percolation::Percolation(size_t dimension)
    : dimension_(dimension),
      opened_count_(0),
      virtual_top_(dimension * dimension),
      virtual_bottom_(dimension * dimension + 1),
      opened_(dimension * dimension, false),
      percolation_dsu_(dimension * dimension + 2),
      fullness_dsu_(dimension * dimension + 1)
{
    if (dimension == 0)
    {
        throw std::invalid_argument("dimension must be positive");
    }
}

void Percolation::open(size_t row, size_t column)
{
    validate_coordinates(row, column);

    const size_t current = get_index(row, column);

    if (opened_[current])
    {
        return;
    }

    opened_[current] = true;
    ++opened_count_;

    if (row == 0)
    {
        percolation_dsu_.unite(current, virtual_top_);
        fullness_dsu_.unite(current, virtual_top_);
    }

    if (row + 1 == dimension_)
    {
        percolation_dsu_.unite(current, virtual_bottom_);
    }

    if (row > 0 && is_open(row - 1, column))
    {
        const size_t neighbour = get_index(row - 1, column);
        percolation_dsu_.unite(current, neighbour);
        fullness_dsu_.unite(current, neighbour);
    }

    if (row + 1 < dimension_ && is_open(row + 1, column))
    {
        const size_t neighbour = get_index(row + 1, column);
        percolation_dsu_.unite(current, neighbour);
        fullness_dsu_.unite(current, neighbour);
    }

    if (column > 0 && is_open(row, column - 1))
    {
        const size_t neighbour = get_index(row, column - 1);
        percolation_dsu_.unite(current, neighbour);
        fullness_dsu_.unite(current, neighbour);
    }

    if (column + 1 < dimension_ && is_open(row, column + 1))
    {
        const size_t neighbour = get_index(row, column + 1);
        percolation_dsu_.unite(current, neighbour);
        fullness_dsu_.unite(current, neighbour);
    }
}

bool Percolation::is_open(size_t row, size_t column) const
{
    validate_coordinates(row, column);

    return opened_[get_index(row, column)];
}

bool Percolation::is_full(size_t row, size_t column) const
{
    validate_coordinates(row, column);

    const size_t current = get_index(row, column);

    return opened_[current] && fullness_dsu_.connected(current, virtual_top_);
}

bool Percolation::percolates() const
{
    return percolation_dsu_.connected(virtual_top_, virtual_bottom_);
}

size_t Percolation::get_open_count() const
{
    return opened_count_;
}

size_t Percolation::get_index(size_t row, size_t column) const
{
    return row * dimension_ + column;
}

void Percolation::validate_coordinates(size_t row, size_t column) const
{
    if (row >= dimension_ || column >= dimension_)
    {
        throw std::out_of_range("cell coordinates are out of range");
    }
}
