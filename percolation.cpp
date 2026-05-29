#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <cmath>
#include <algorithm>
#include <stdexcept>

class DSU {
private:
    std::vector<int> parent;
    std::vector<int> size;

public:
    DSU(int n) {
        parent.resize(n);
        size.assign(n, 1);
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }

    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            if (size[root_i] < size[root_j]) std::swap(root_i, root_j);
            parent[root_j] = root_i;
            size[root_i] += size[root_j];
        }
    }

    bool connected(int i, int j) {
        return find(i) == find(j);
    }
};

class Percolation {
private:
    int n;
    std::vector<bool> opened;
    int open_count;
    DSU dsu;
    int virtual_top;
    int virtual_bottom;

    int get_index(int row, int col) const {
        return row * n + col;
    }

public:
    Percolation(int dimension) 
        : n(dimension), 
          opened(n * n, false), 
          open_count(0), 
          dsu(n * n + 2), 
          virtual_top(n * n), 
          virtual_bottom(n * n + 1) {}

    void open(int row, int col) {
        int idx = get_index(row, col);
        if (opened[idx]) return;
        
        opened[idx] = true;
        open_count++;

        if (row == 0) dsu.unite(idx, virtual_top);
        if (row == n - 1) dsu.unite(idx, virtual_bottom);

        int dr[] = {-1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1};
        for (int i = 0; i < 4; ++i) {
            int nr = row + dr[i];
            int nc = col + dc[i];
            
            if (nr >= 0 && nr < n && nc >= 0 && nc < n && opened[get_index(nr, nc)]) {
                dsu.unite(idx, get_index(nr, nc));
            }
        }
    }

    bool percolates() {
        return dsu.connected(virtual_top, virtual_bottom);
    }

    int get_open_count() const {
        return open_count;
    }
};

struct PercolationStats {
    size_t dimension;
    size_t trials;
    
    std::vector<double> thresholds;
    double mean_val = 0.0;
    double std_dev_val = 0.0;

    PercolationStats(size_t dimension, size_t trials) : dimension(dimension), trials(trials) {
        if (dimension <= 0 || trials <= 0) {
            throw std::invalid_argument("Размерность и количество попыток должны быть > 0");
        }
    }

    void execute() {
        thresholds.clear();
        thresholds.reserve(trials);
        
        std::mt19937 gen(std::random_device{}());

        for (size_t t = 0; t < trials; ++t) {
            Percolation perc(dimension);
            
            std::vector<std::pair<int, int>> cells;
            cells.reserve(dimension * dimension);
            for (size_t r = 0; r < dimension; ++r) {
                for (size_t c = 0; c < dimension; ++c) {
                    cells.emplace_back(r, c);
                }
            }
            
            std::shuffle(cells.begin(), cells.end(), gen);

            size_t idx = 0;
            while (!perc.percolates() && idx < cells.size()) {
                perc.open(cells[idx].first, cells[idx].second);
                idx++;
            }

            double threshold = static_cast<double>(perc.get_open_count()) / (dimension * dimension);
            thresholds.push_back(threshold);
        }

        double sum = std::accumulate(thresholds.begin(), thresholds.end(), 0.0);
        mean_val = sum / trials;

        double sq_sum = 0.0;
        for (double val : thresholds) {
            sq_sum += (val - mean_val) * (val - mean_val);
        }
        std_dev_val = (trials > 1) ? std::sqrt(sq_sum / (trials - 1)) : 0.0;
    }

    double get_mean() const { 
        return mean_val; 
    }
    
    double get_standard_deviation() const { 
        return std_dev_val; 
    }
    
    double get_confidence_low() const { 
        return mean_val - (1.96 * std_dev_val) / std::sqrt(trials); 
    }
    
    double get_confidence_high() const { 
        return mean_val + (1.96 * std_dev_val) / std::sqrt(trials); 
    }
};

// приемр 
int main() {
    size_t grid_size = 200;
    size_t T = 100;
    
    std::cout << "Running percolation" << grid_size << "x" << grid_size 
              << " grid (" << T << " trials)...\n";


    PercolationStats stats(grid_size, T);
    stats.execute();

    std::cout << "Mean:   " << stats.get_mean() << "\n";
    std::cout << "Standard Deviation:        " << stats.get_standard_deviation() << "\n";
    std::cout << "95% Confidence Interval:   [" 
              << stats.get_confidence_low() << ", " 
              << stats.get_confidence_high() << "]\n";

    return 0;
}