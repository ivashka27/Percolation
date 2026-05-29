#pragma once
#include <vector>
#include <numeric>

class DisjointSetUnion {
private:
    std::vector<size_t> parent;
    std::vector<size_t> rank;
public:
    DisjointSetUnion(size_t n) : parent(n), rank(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    size_t find(size_t i) {
        if (parent[i] == i)
            return i;
        return parent[i] = find(parent[i]);
    }

    void unite(size_t i, size_t j) {
        size_t root_i = find(i);
        size_t root_j = find(j);
        if (root_i != root_j) {
            if (rank[root_i] < rank[root_j]) {
                parent[root_i] = root_j;
            } else if (rank[root_i] > rank[root_j]) {
                parent[root_j] = root_i;
            } else {
                parent[root_j] = root_i;
                rank[root_i]++;
            }
        }
    }

    bool connected(size_t i, size_t j) {
        return find(i) == find(j);
    }
};