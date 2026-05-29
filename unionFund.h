#pragma once

#include <vector>

class DSU
{
private:
    std::vector<size_t> parent;
    std::vector<size_t> rank;

public:
    explicit DSU(size_t n)
        : parent(n), rank(n, 0)
    {
        for (size_t i = 0; i < n; ++i)
        {
            parent[i] = i;
        }
    }

    size_t find(size_t v)
    {
        if (parent[v] == v)
        {
            return v;
        }

        return parent[v] = find(parent[v]);
    }

    void unite(size_t a, size_t b)
    {
        a = find(a);
        b = find(b);

        if (a == b)
        {
            return;
        }

        if (rank[a] < rank[b])
        {
            std::swap(a, b);
        }

        parent[b] = a;

        if (rank[a] == rank[b])
        {
            ++rank[a];
        }
    }

    bool connected(size_t a, size_t b)
    {
        return find(a) == find(b);
    }
};