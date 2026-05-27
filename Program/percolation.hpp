#include <vector>
#include <random>
#include <algorithm>
#include <cmath>

#ifndef PERCOLATION_HPP
#define PERCOLATION_HPP

class UnionFind {
private:

    std::vector<int> parent;
    std::vector<int> rank;

public:

    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; ++i) 
            parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    };

    void unite(int x, int y) {
        int rootX = find(x);
        int rootY = find(y);

        if (rootX == rootY) return;
        else if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } else {
            parent[rootY] = rootX;
            ++rank[rootX];
        }
    }

    bool connected(int x, int y) {
        return (find(x) == find(y));
    }
};

class Percolation {
private:

    int n;
    std::vector<bool> openSites;
    UnionFind uf;
    int virtualTop, virtualBottom;
    int openCount;

public:

    Percolation(int n) 
        : n(n), openSites(n * n, false), uf(n * n + 2) {
        virtualTop = n * n;
        virtualBottom = n * n + 1;
        openCount = 0;
    }

    int index(int row, int col) {
        return row * n + col;
    }

    bool isOpen(int row, int col) {
        return openSites[index(row, col)];
    }

    void open(int row, int col) {
        if (isOpen(row, col)) return;

        openSites[index(row, col)] = true;
        ++openCount;
        
        if (row > 0 && isOpen(row - 1, col)) uf.unite(index(row, col), index(row - 1, col));
        if (row < n - 1 && isOpen(row + 1, col)) uf.unite(index(row, col), index(row + 1, col));
        if (col > 0 && isOpen(row, col - 1)) uf.unite(index(row, col), index(row, col - 1));
        if (col < n - 1 && isOpen(row, col + 1)) uf.unite(index(row, col), index(row, col + 1));

        if (row == 0) uf.unite(index(row, col), virtualTop);
        if (row == n - 1) uf.unite(index(row, col), virtualBottom);  
    }

    int numberOfOpenCells() {
        return openCount;
    }

    bool percolates() {
        return uf.connected(virtualTop, virtualBottom);
    }

    bool isFull(int row, int col) {
        return uf.connected(index(row, col), virtualTop);
    }
};

struct PercolationStats {
    size_t dimension, trials;
    double mean, stddev, confidenceLow, confidenceHigh;

    PercolationStats(size_t dim, size_t t) {
        dimension = dim;
        trials = t;
    }

    void execute() {
        std::vector<double> thresholds(trials);
        std::random_device rd;
        std::mt19937 g(rd());

        for (size_t t = 0; t < trials; ++t) {
            Percolation p(dimension);
            std::vector<std::pair<int,int>> cells;

            for (int row = 0; row < dimension; ++row) {
                for (int col = 0; col < dimension; ++col) {
                    cells.push_back({row, col});
                }
            }

            std::shuffle(cells.begin(), cells.end(), g);

            for (size_t i = 0; i < cells.size(); ++i) {
                int row = cells[i].first;
                int col = cells[i].second;
                p.open(row, col);

                if (p.percolates()) {
                    thresholds[t] = (double)p.numberOfOpenCells() / (dimension * dimension);
                    break;
                }
            }
        }
        double sum_mean = 0.0, sq_sum = 0.0, dev;
        for (size_t t = 0; t < trials; ++t) {
            sum_mean += thresholds[t];
        }
        mean = sum_mean / trials;
        for (size_t t = 0; t < trials; ++t) {
            sq_sum += std::pow(thresholds[t] - mean, 2);
        }
        stddev = std::sqrt(sq_sum / (trials - 1));
        dev = 1.96 * stddev / std::sqrt(trials);

        confidenceLow = mean - dev;
        confidenceHigh = mean + dev;

    }

    double get_mean() const {
        return mean;
    }

    double get_standard_deviation() const {
        return stddev;
    }

    double get_confidence_low() const {
        return confidenceLow;
    }

    double get_confidence_high() const {
        return confidenceHigh;
    }
};

#endif // PERCOLATION_HPP