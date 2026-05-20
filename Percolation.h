#pragma once

#include <cstddef>
#include <vector>

class Percolation
{
public:
    explicit Percolation(size_t dimension);

    void open(size_t row, size_t column);

    bool is_open(size_t row, size_t column) const;

    bool is_full(size_t row, size_t column) const;

    bool percolates() const;

    size_t get_open_count() const;

private:
    class DisjointSetUnion
    {
    public:
        explicit DisjointSetUnion(size_t size);

        size_t find(size_t vertex);

        void unite(size_t first, size_t second);

        bool connected(size_t first, size_t second);

    private:
        std::vector<size_t> parent_;
        std::vector<size_t> rank_;
    };

    size_t get_index(size_t row, size_t column) const;

    void validate_coordinates(size_t row, size_t column) const;

    size_t dimension_;
    size_t opened_count_;

    size_t virtual_top_;
    size_t virtual_bottom_;

    std::vector<bool> opened_;

    mutable DisjointSetUnion percolation_dsu_;
    mutable DisjointSetUnion fullness_dsu_;
};
