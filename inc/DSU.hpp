#pragma once
#include <vector>

class DSU {
private:
    std::vector<int> parent;
    std::vector<int> size;

public:
    explicit DSU(int n);
    int find(int i);
    void unite(int i, int j);
    bool connected(int i, int j);
};