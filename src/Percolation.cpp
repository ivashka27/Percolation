#include "Percolation.hpp"

Percolation::Percolation(size_t n): 
    m_n(n), 
    m_size(n * n), 
    m_open(m_size, false), 
    m_parent(m_size), 
    m_has_top_connection(m_size, false), 
    m_has_bottom_connection(m_size, false), 
    m_open_count(0)
{
    for (size_t i = 0; i < m_size; ++i)
    {
        m_parent[i] = i;
    }
}

size_t Percolation::get_index(size_t row, size_t col) const
{
    return row * m_n + col;
}

size_t Percolation::find(size_t x)
{
    if (m_parent[x] != x)
    {
        m_parent[x] = find(m_parent[x]);
    }
    return m_parent[x];
}

void Percolation::unite(size_t a, size_t b)
{
    size_t root_a = find(a);
    size_t root_b = find(b);
    if (root_a == root_b)
    {
        return;
    }
    m_parent[root_b] = root_a;
    m_has_top_connection[root_a] = m_has_top_connection[root_a] || m_has_top_connection[root_b];
    m_has_bottom_connection[root_a] = m_has_bottom_connection[root_a] || m_has_bottom_connection[root_b];
}

void Percolation::open(size_t row, size_t col)
{
    size_t idx = get_index(row, col);
    if (m_open[idx])
    {
        return;
    }
    m_open[idx] = true;
    ++m_open_count;

    if (row == 0)
    {
        m_has_top_connection[idx] = true;
    }
    if (row == m_n - 1)
    {
        m_has_bottom_connection[idx] = true;
    }

    if (col > 0 && is_open(row, col - 1))
    {
        unite(idx, get_index(row, col - 1));
    }
    if (col + 1 < m_n && is_open(row, col + 1))
    {
        unite(idx, get_index(row, col + 1));
    }
    if (row > 0 && is_open(row - 1, col))
    {
        unite(idx, get_index(row - 1, col));
    }
    if (row + 1 < m_n && is_open(row + 1, col))
    {
        unite(idx, get_index(row + 1, col));
    }
}

bool Percolation::is_open(size_t row, size_t col) const
{
    return m_open[get_index(row, col)];
}

bool Percolation::percolates()
{
    for (size_t i = 0; i < m_size; ++i)
    {
        size_t root = find(i);
        if (m_has_top_connection[root] && m_has_bottom_connection[root])
        {
            return true;
        }
    }
    return false;
}

size_t Percolation::get_open_count() const
{
    return m_open_count;
}

size_t Percolation::get_dimension() const
{
    return m_n;
}