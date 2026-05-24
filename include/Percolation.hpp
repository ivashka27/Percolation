#ifndef PERCOLATION_HPP
#define PERCOLATION_HPP

#include <vector>

class Percolation
{
public:
    Percolation(size_t n);

    void open(size_t row, size_t col);

    bool is_open(size_t row, size_t col) const;

    bool percolates();

    size_t get_open_count() const;

    size_t get_dimension() const;

private:
    size_t m_n;
    size_t m_size;
    std::vector<bool> m_open;
    std::vector<size_t> m_parent;
    std::vector<bool> m_has_top_connection;
    std::vector<bool> m_has_bottom_connection;
    size_t m_open_count;

    size_t get_index(size_t row, size_t col) const;

    size_t find(size_t x);

    void unite(size_t a, size_t b);
};

#endif