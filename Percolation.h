#pragma once

#include <cstddef>
#include <vector>

// percolation model
struct Percolation
{
    // in the begining all cells are blocked
    Percolation(size_t dimension);

    // open the cell if it not opened yet
    void open(size_t row, size_t column);

    // return true when the cell is open
    bool is_open(size_t row, size_t column) const;

    // return true if the cell is full (the water can reach it from the top)
    bool is_full(size_t row, size_t column) const;

    // how many cells we opened already
    size_t number_of_opened() const;

    // check if the water go from the top row down to the bottom row
    bool is_percolating() const;

private:
    // walk over the field from the top and mark every cell where water can be
    void splash_water_from_top(std::vector<std::vector<bool>> & wet) const;

    size_t n;
    size_t opened;                        // counter of opened cells
    std::vector<std::vector<bool>> field; // true = open, false = blocked
};
