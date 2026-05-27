#include "DSU.hpp"
#include <numeric>

DSU::DSU(int n) : parent(n), size(n, 1) {
    std::iota(parent.begin(), parent.end(), 0);
}

int DSU::find(int i) {
    if (parent[i] == i)
        return i;
    return parent[i] = find(parent[i]); 
}

void DSU::unite(int i, int j) {
    int root_i = find(i);
    int root_j = find(j);
    
    if (root_i != root_j) {
        if (size[root_i] < size[root_j]) {
            parent[root_i] = root_j;
            size[root_j] += size[root_i];
        } else {
            parent[root_j] = root_i;
            size[root_i] += size[root_j];
        }
    }
}

bool DSU::connected(int i, int j) {
    return find(i) == find(j);
}