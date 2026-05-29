#pragma once

#include <cstddef>
#include <vector>

class DisjointSet
{
public:
    explicit DisjointSet(std::size_t size);

    std::size_t find(std::size_t value);
    void unite(std::size_t first, std::size_t second);
    bool connected(std::size_t first, std::size_t second);
    bool connected(std::size_t first, std::size_t second) const;

private:
    std::size_t find_const(std::size_t value) const;

    std::vector<std::size_t> parent;
    std::vector<std::size_t> tree_size;
};

class Percolation
{
public:
    explicit Percolation(std::size_t dimension);

    void open(std::size_t row, std::size_t column);
    bool is_open(std::size_t row, std::size_t column) const;
    bool is_full(std::size_t row, std::size_t column) const;
    bool has_percolation() const;
    bool percolates() const;

    std::size_t get_dimension() const;
    std::size_t get_open_count() const;

private:
    std::size_t index(std::size_t row, std::size_t column) const;
    void check_cell(std::size_t row, std::size_t column) const;
    void connect_if_open(std::size_t row, std::size_t column, std::size_t current_index);

    std::size_t grid_size;
    std::size_t open_count;
    std::size_t top_index;
    std::size_t bottom_index;
    std::vector<bool> opened;
    DisjointSet percolation_set;
    DisjointSet fullness_set;
};
