#pragma once

#include <cstddef>
#include <vector>

namespace percolation_detail {

class UnionFind {
public:
    explicit UnionFind(std::size_t size);

    void unite(std::size_t left, std::size_t right);
    bool connected(std::size_t left, std::size_t right) const;

private:
    std::size_t find(std::size_t site) const;

    mutable std::vector<std::size_t> parents_;
    std::vector<std::size_t> sizes_;
};

} // namespace percolation_detail
