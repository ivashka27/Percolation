#pragma once
#include <cstddef>
#include <vector>

class Percolation {
public:
    explicit Percolation(size_t dimension);
    
    void open(size_t row, size_t col);
    bool is_open(size_t row, size_t col) const;
    bool has_percolation();
    size_t number_of_open_cells() const;

private:
    struct UnionFind {
        std::vector<size_t> parent;
        std::vector<size_t> rank;
        
        explicit UnionFind(size_t n);
        size_t find(size_t x);
        void union_sets(size_t x, size_t y);
    };

    size_t dimension;
    std::vector<bool> open_grid;
    size_t open_count;
    UnionFind uf;
    size_t top_virtual;
    size_t bottom_virtual;

    size_t index(size_t row, size_t col) const;
    void validate(size_t row, size_t col) const;
};