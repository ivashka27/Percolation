#include "percolation/UnionFind.hpp"

#include <numeric>

namespace percolation_detail {

UnionFind::UnionFind(std::size_t size)
    : parents_(size),
      sizes_(size, 1)
{
    std::iota(parents_.begin(), parents_.end(), 0);
}

void UnionFind::unite(std::size_t left, std::size_t right)
{
    std::size_t left_root = find(left);
    std::size_t right_root = find(right);

    if (left_root == right_root) {
        return;
    }

    if (sizes_[left_root] < sizes_[right_root]) {
        std::swap(left_root, right_root);
    }

    parents_[right_root] = left_root;
    sizes_[left_root] += sizes_[right_root];
}

bool UnionFind::connected(std::size_t left, std::size_t right) const
{
    return find(left) == find(right);
}

std::size_t UnionFind::find(std::size_t site) const
{
    while (site != parents_[site]) {
        parents_[site] = parents_[parents_[site]];
        site = parents_[site];
    }

    return site;
}

} 
