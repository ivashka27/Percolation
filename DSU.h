#pragma once
#include <vector>
#include <numeric>

class DSU {
private:
    std::vector<size_t> leader;
    std::vector<size_t> tree_depth;

public:
    explicit DSU(size_t elements_count)
        : leader(elements_count), tree_depth(elements_count, 0) 
    {
        std::iota(leader.begin(), leader.end(), 0);
    }

    size_t find_root(size_t index) {
        if (leader[index] == index) {
            return index;
        }
        return leader[index] = find_root(leader[index]);
    }

    void merge(size_t first, size_t second) {
        size_t root_a = find_root(first);
        size_t root_b = find_root(second);

        if (root_a != root_b) {
            if (tree_depth[root_a] < tree_depth[root_b]) {
                leader[root_a] = root_b;
            } else if (tree_depth[root_a] > tree_depth[root_b]) {
                leader[root_b] = root_a;
            } else {
                leader[root_b] = root_a;
                tree_depth[root_a]++;
            }
        }
    }

    bool are_connected(size_t first, size_t second) {
        return find_root(first) == find_root(second);
    }
};