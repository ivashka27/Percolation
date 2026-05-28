#pragma once

#include "percolation/UnionFind.hpp"

#include <cstddef>
#include <vector>

class Percolation {
public:
    explicit Percolation(std::size_t dimension);

    void open(std::size_t row, std::size_t col);
    bool is_open(std::size_t row, std::size_t col) const;
    bool is_full(std::size_t row, std::size_t col) const;
    bool percolates() const;
    std::size_t get_open_sites_count() const;

private:
    void validate_indexes(std::size_t row, std::size_t col) const;
    std::size_t to_index(std::size_t row, std::size_t col) const;
    void connect_if_open(std::size_t site, std::size_t row, std::size_t col);

    std::size_t dimension_;
    std::size_t open_sites_count_;
    std::size_t virtual_top_;
    std::size_t virtual_bottom_;
    std::vector<bool> open_sites_;
    percolation_detail::UnionFind percolation_union_find_;
    percolation_detail::UnionFind fullness_union_find_;
};
