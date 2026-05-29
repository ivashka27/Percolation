#pragma once
#include <vector>
#include <cstddef>

class Percolation {
private:
    class DSU {
    private:
        std::vector<size_t> parent;
        std::vector<size_t> sz;
    public:
        explicit DSU(size_t n);
        size_t find(size_t v);
        void unite(size_t a, size_t b);
        bool connected(size_t a, size_t b);
    };

    size_t n;
    std::vector<bool> opened;
    DSU dsu;
    size_t virtual_top;
    size_t virtual_bottom;
    size_t opened_count;
    size_t index(size_t row, size_t col) const;
    bool valid(size_t row, size_t col) const;
    
public:
    explicit Percolation(size_t dimension);
    void open(size_t row, size_t col);
    bool is_open(size_t row, size_t col) const;
    bool percolates();
    size_t get_opened_count() const;
};