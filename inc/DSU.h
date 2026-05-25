#pragma once
#include <vector>

class DSU {
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    explicit DSU(int size);
    int find(int x);
    void merge(int a, int b);
    bool is_connected(int a, int b);
};
