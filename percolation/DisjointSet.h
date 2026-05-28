#pragma once

#include <cstddef>
#include <numeric>
#include <vector>

class DisjointSet {
public:
    explicit DisjointSet(size_t size) : parent_(size), size_(size, 1) {
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    size_t find(size_t x) const {
        while (x != parent_[x]) {
            parent_[x] = parent_[parent_[x]];
            x = parent_[x];
        }
        return x;
    }

    void unite(size_t a, size_t b) {
        a = find(a);
        b = find(b);
        if (a == b) {
            return;
        }
        if (size_[a] < size_[b]) {
            std::swap(a, b);
        }
        parent_[b] = a;
        size_[a] += size_[b];
    }

    bool connected(size_t a, size_t b) const {
        return find(a) == find(b);
    }

private:
    mutable std::vector<size_t> parent_;
    std::vector<size_t> size_;
};
