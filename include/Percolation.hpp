#pragma once
#include <vector>
#include <random>

class Percolation {
private:
    size_t n;
    std::vector<bool> open_sites;
    std::vector<int> parent;
    std::vector<int> sz;
    bool percolates_flag;
    const int TOP_VIRTUAL = 0;
    int BOTTOM_VIRTUAL;
    
    int get_index(size_t row, size_t col) const {
        return row * n + col + 1;
    }
    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    }
    
    void unite(int a, int b) {
        int root_a = find(a);
        int root_b = find(b);
        if (root_a == root_b) return;
        
        if (sz[root_a] < sz[root_b]) {
            parent[root_a] = root_b;
            sz[root_b] += sz[root_a];
        } else {
            parent[root_b] = root_a;
            sz[root_a] += sz[root_b];
        }
    }
    
    bool connected(int a, int b) {
        return find(a) == find(b);
    }
    
    void connect_neighbors(size_t row, size_t col) {
        int current = get_index(row, col);

        if (row > 0 && is_open(row - 1, col)) {
            unite(current, get_index(row - 1, col));
        }
        if (row + 1 < n && is_open(row + 1, col)) {
            unite(current, get_index(row + 1, col));
        }
        if (col > 0 && is_open(row, col - 1)) {
            unite(current, get_index(row, col - 1));
        }
        if (col + 1 < n && is_open(row, col + 1)) {
            unite(current, get_index(row, col + 1));
        }
        
        if (row == 0) {
            unite(current, TOP_VIRTUAL);
        }
        if (row == n - 1) {
            unite(current, BOTTOM_VIRTUAL);
        }
        
        if (!percolates_flag && connected(TOP_VIRTUAL, BOTTOM_VIRTUAL)) {
            percolates_flag = true;
        }
    }
    
public:
    Percolation(size_t dimension);
    void open(size_t row, size_t col);
    bool is_open(size_t row, size_t col) const;
    bool percolates() const;
    size_t get_number_of_open_sites() const;
};