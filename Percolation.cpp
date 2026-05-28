#include "percolation/Percolation.hpp"

#include <stdexcept>

Percolation::Percolation(std::size_t dimension)
    : dimension_(dimension),
      open_sites_count_(0),
      virtual_top_(dimension * dimension),
      virtual_bottom_(dimension * dimension + 1),
      open_sites_(dimension * dimension, false),
      percolation_union_find_(dimension * dimension + 2),
      fullness_union_find_(dimension * dimension + 1)
{
    if (dimension == 0) {
        throw std::invalid_argument("Percolation dimension must be greater than zero");
    }
}

void Percolation::open(std::size_t row, std::size_t col)
{
    validate_indexes(row, col);

    const std::size_t site = to_index(row, col);
    if (open_sites_[site]) {
        return;
    }

    open_sites_[site] = true;
    ++open_sites_count_;

    if (row == 0) {
        percolation_union_find_.unite(site, virtual_top_);
        fullness_union_find_.unite(site, virtual_top_);
    }

    if (row + 1 == dimension_) {
        percolation_union_find_.unite(site, virtual_bottom_);
    }

    if (row > 0) {
        connect_if_open(site, row - 1, col);
    }
    if (row + 1 < dimension_) {
        connect_if_open(site, row + 1, col);
    }
    if (col > 0) {
        connect_if_open(site, row, col - 1);
    }
    if (col + 1 < dimension_) {
        connect_if_open(site, row, col + 1);
    }
}

bool Percolation::is_open(std::size_t row, std::size_t col) const
{
    validate_indexes(row, col);
    return open_sites_[to_index(row, col)];
}

bool Percolation::is_full(std::size_t row, std::size_t col) const
{
    validate_indexes(row, col);

    const std::size_t site = to_index(row, col);
    return open_sites_[site] && fullness_union_find_.connected(site, virtual_top_);
}

bool Percolation::percolates() const
{
    return percolation_union_find_.connected(virtual_top_, virtual_bottom_);
}

std::size_t Percolation::get_open_sites_count() const
{
    return open_sites_count_;
}

void Percolation::validate_indexes(std::size_t row, std::size_t col) const
{
    if (row >= dimension_ || col >= dimension_) {
        throw std::out_of_range("Percolation indexes are outside of the grid");
    }
}

std::size_t Percolation::to_index(std::size_t row, std::size_t col) const
{
    return row * dimension_ + col;
}

void Percolation::connect_if_open(std::size_t site, std::size_t row, std::size_t col)
{
    const std::size_t neighbor = to_index(row, col);
    if (!open_sites_[neighbor]) {
        return;
    }

    percolation_union_find_.unite(site, neighbor);
    fullness_union_find_.unite(site, neighbor);
}
