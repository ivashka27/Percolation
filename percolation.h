#pragma once
#include <vector>
#include <stdexcept>

class WeightedQuickUnion {
private:
    mutable std::vector<int> parent; 
    mutable std::vector<int> sz;
    int count;

public:
    WeightedQuickUnion(int n);
    int find(int p) const;          
    void connect(int p, int q);
    bool connected(int p, int q) const; 
    int getCount() const;
};

class Percolation {
private:
    int dimension;
    std::vector<bool> grid;
    WeightedQuickUnion uf;
    WeightedQuickUnion uf_top;

    int top_virtual;
    int bottom_virtual;
    int num_open_sites;

    int to1D(int row, int col) const;

public:
    Percolation(int dimension);
    void open(int row, int col);
    bool isOpen(int row, int col) const;
    bool isFull(int row, int col) const;
    int numberOfOpenSites() const;
    bool percolates() const;
};
