#pragma once
#include "DSU.hpp" 
#include <vector>
#include <cstddef>

class Percolation {
private:
    std::size_t n;
    std::vector<bool> opened;
    std::size_t open_sites_count;
    
    DSU dsu_perc; 
    DSU dsu_full; 
    
    int virtual_top;
    int virtual_bottom;

    int get_index(std::size_t row, std::size_t col) const;
    void check_bounds(std::size_t row, std::size_t col) const;

public:
    explicit Percolation(std::size_t dimension);
    
    void open(std::size_t row, std::size_t col);
    bool is_open(std::size_t row, std::size_t col) const;
    bool is_full(std::size_t row, std::size_t col);
    std::size_t get_number_of_open_cells() const;
    bool percolates();
};