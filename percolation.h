#pragma once

#include <cstddef>
#include <vector>

class Percolation
{
private:
    std::size_t dimension;
    std::vector<std::vector<bool>> grid;
    std::size_t open_count;

    void check_coordinates(std::size_t row, std::size_t col) const;
public:
    explicit Percolation(std::size_t dimension);

    void open(std::size_t row, std::size_t col);
    bool is_open(std::size_t row, std::size_t col) const;
    bool is_full(std::size_t row, std::size_t col) const;
    bool percolates() const;

    std::size_t get_open_count() const;
    std::size_t get_dimension() const;
};

struct PercolationStats
{
private:
    std::size_t dimension;
    std::size_t trials;
    std::vector<double> thresholds;

    double mean;
    double standard_deviation;
    double confidence_low;
    double confidence_high;
public:
    PercolationStats(std::size_t dimension, std::size_t trials);

    double get_mean() const;
    double get_standard_deviation() const;

    double get_confidence_low() const;
    double get_confidence_high() const;

    void execute();
};
