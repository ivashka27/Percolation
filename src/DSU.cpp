#include "../inc/DSU.h"

DSU::DSU(int size) {
    parent.resize(size);
    rank.resize(size, 0);
    for (int i = 0; i < size; ++i) {
        parent[i] = i;
    }
}

int DSU::find(int x) {
    if (parent[x] != x) {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

void DSU::merge(int a, int b) {
    int root_a = find(a);
    int root_b = find(b);

    if (root_a == root_b) return;

    if (rank[root_a] < rank[root_b]) {
        parent[root_a] = root_b;
    } else if (rank[root_a] > rank[root_b]) {
        parent[root_b] = root_a;
    } else {
        parent[root_b] = root_a;
        rank[root_a]++;
    }
}

bool DSU::is_connected(int a, int b) {
    return find(a) == find(b);
}
