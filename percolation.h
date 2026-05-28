#ifndef PERCOLATION_H
#define PERCOLATION_H
#include <vector>

class Percolation {
public:
    Percolation(size_t n);
    void open(size_t row, size_t col);
    bool isOpen(size_t row, size_t col) const;
    bool isFull(size_t row, size_t col) const;
    bool percolates() const;
    size_t numberOfOpenSites() const;
private:
    size_t n_;
    std::vector<std::vector<bool>> grid_;
    std::vector<std::vector<bool>> full_;
    void fill(size_t row, size_t col);
};
#endif