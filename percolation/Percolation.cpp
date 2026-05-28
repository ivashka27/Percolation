#include "Percolation.h"

#include <stdexcept>

Percolation::Percolation(size_t dimension)
    : dimension_(dimension),
      virtual_top_(dimension * dimension),
      virtual_bottom_(dimension * dimension + 1),
      open_sites_count_(0),
      open_sites_(dimension * dimension, false),
      flow_(dimension * dimension + 2),
      fullness_(dimension * dimension + 1) {
    if (dimension_ == 0) {
        throw std::invalid_argument("dimension must be positive");
    }
}

void Percolation::open(size_t row, size_t col) {
    validate(row, col);

    const size_t current = index(row, col);
    if (open_sites_[current]) {
        return;
    }

    open_sites_[current] = true;
    ++open_sites_count_;

    if (row == 0) {
        flow_.unite(current, virtual_top_);
        fullness_.unite(current, virtual_top_);
    }
    if (row + 1 == dimension_) {
        flow_.unite(current, virtual_bottom_);
    }

    if (row > 0 && is_open(row - 1, col)) {
        const size_t up = index(row - 1, col);
        flow_.unite(current, up);
        fullness_.unite(current, up);
    }
    if (row + 1 < dimension_ && is_open(row + 1, col)) {
        const size_t down = index(row + 1, col);
        flow_.unite(current, down);
        fullness_.unite(current, down);
    }
    if (col > 0 && is_open(row, col - 1)) {
        const size_t left = index(row, col - 1);
        flow_.unite(current, left);
        fullness_.unite(current, left);
    }
    if (col + 1 < dimension_ && is_open(row, col + 1)) {
        const size_t right = index(row, col + 1);
        flow_.unite(current, right);
        fullness_.unite(current, right);
    }
}

bool Percolation::is_open(size_t row, size_t col) const {
    validate(row, col);
    return open_sites_[index(row, col)];
}

bool Percolation::is_full(size_t row, size_t col) const {
    validate(row, col);
    return is_open(row, col) && fullness_.connected(index(row, col), virtual_top_);
}

bool Percolation::percolates() const {
    return flow_.connected(virtual_top_, virtual_bottom_);
}

size_t Percolation::get_dimension() const {
    return dimension_;
}

size_t Percolation::get_open_sites_count() const {
    return open_sites_count_;
}

size_t Percolation::index(size_t row, size_t col) const {
    return row * dimension_ + col;
}

void Percolation::validate(size_t row, size_t col) const {
    if (row >= dimension_ || col >= dimension_) {
        throw std::out_of_range("grid coordinates are out of range");
    }
}
