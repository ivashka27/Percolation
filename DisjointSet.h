#pragma once

#include <vector>
#include <cstddef>

class DisjointSet
{
private:
    std::vector<size_t> parent;
    std::vector<size_t> set_size;

public:
    explicit DisjointSet(size_t n)
        : parent(n), set_size(n, 1)
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

        parent[v] = find(parent[v]);
        return parent[v];
    }

    void unite(size_t a, size_t b)
    {
        a = find(a);
        b = find(b);

        if (a == b)
        {
            return;
        }

        if (set_size[a] < set_size[b])
        {
            size_t temp = a;
            a = b;
            b = temp;
        }

        parent[b] = a;
        set_size[a] += set_size[b];
    }

    bool connected(size_t a, size_t b)
    {
        return find(a) == find(b);
    }
};