#pragma once

#include <cstddef>
#include <vector>

class Percolation
{
public:
    explicit Percolation(std::size_t dimension);

    void open(std::size_t row, std::size_t column);
    bool is_open(std::size_t row, std::size_t column) const;
    bool is_full(std::size_t row, std::size_t column) const;
    bool percolates() const;
    std::size_t get_dimension() const;
    std::size_t get_open_sites() const;

private:
    std::size_t index(std::size_t row, std::size_t column) const;
    void check_coordinates(std::size_t row, std::size_t column) const;

private:
    std::size_t m_dimension;
    std::vector<bool> m_open;
    std::size_t m_open_sites;
};
