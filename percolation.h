#pragma once

#include <vector>
#include <random>
#include <stdexcept>
#include "unionFund.h"

class Percolation
{
private:
    size_t n;

    std::vector<bool> opened;

    size_t open_sites = 0;

    DSU dsu;

    size_t virtual_top;
    size_t virtual_bottom;

    size_t index(size_t row, size_t col) const
    {
        return row * n + col;
    }

    bool valid(size_t row, size_t col) const
    {
        return row < n && col < n;
    }

public:
    explicit Percolation(size_t dimension)
        : n(dimension),
          opened(n * n, false),
          dsu(n * n + 2),
          virtual_top(n * n),
          virtual_bottom(n * n + 1)
    {
    }

    void open(size_t row, size_t col)
    {
        if (!valid(row, col))
        {
            throw std::out_of_range("Invalid index");
        }

        size_t current = index(row, col);

        if (opened[current])
        {
            return;
        }

        opened[current] = true;
        ++open_sites;

        if (row == 0)
        {
            dsu.unite(current, virtual_top);
        }

        if (row == n - 1)
        {
            dsu.unite(current, virtual_bottom);
        }

        const int dr[] = {-1, 1, 0, 0};
        const int dc[] = {0, 0, -1, 1};

        for (int k = 0; k < 4; ++k)
        {
            int nr = static_cast<int>(row) + dr[k];
            int nc = static_cast<int>(col) + dc[k];

            if (nr < 0 || nc < 0)
            {
                continue;
            }

            if (nr >= static_cast<int>(n) ||
                nc >= static_cast<int>(n))
            {
                continue;
            }

            size_t neighbour =
                index(static_cast<size_t>(nr),
                      static_cast<size_t>(nc));

            if (opened[neighbour])
            {
                dsu.unite(current, neighbour);
            }
        }
    }

    bool is_open(size_t row, size_t col) const
    {
        return opened[index(row, col)];
    }

    bool percolates()
    {
        return dsu.connected(
            virtual_top,
            virtual_bottom);
    }

    size_t number_of_open_sites() const
    {
        return open_sites;
    }

    void open_random_site()
    {
        static std::mt19937 gen(std::random_device{}());

        std::uniform_int_distribution<size_t>
            dist(0, n * n - 1);

        while (true)
        {
            size_t pos = dist(gen);

            if (!opened[pos])
            {
                open(pos / n, pos % n);
                return;
            }
        }
    }
};