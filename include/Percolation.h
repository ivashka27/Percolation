#ifndef PERCOLATION_H
#define PERCOLATION_H

#include <cstddef>
#include <vector>

class Percolation {
private:
    size_t size;
    std::vector<bool> grid;
    size_t open_sites;
public:
    Percolation(size_t n);
    
    void open(size_t row, size_t col);
    bool is_open(size_t row, size_t col) const;
    size_t get_open_sites_count() const;
    
    bool percolates() const;
};

#endif