#pragma once
#include <vector>
#include <cstddef>

class UnionFind 
{
private:
    std::vector<size_t> parent;
    std::vector<size_t> rank;

public:
    explicit UnionFind(size_t size);

    size_t find(size_t v);
    void unite(size_t a, size_t b);
    bool connected(size_t a, size_t b);
};