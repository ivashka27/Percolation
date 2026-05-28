#include <iostream>
#include <vector>
#include <cmath>
#include <ctime>
#include <cstdlib>

class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    UnionFind(int n) {
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }

    void unite(int a, int b) {
        int root_a = find(a);
        int root_b = find(b);
        
        if (root_a == root_b) return;

        if (rank[root_a] < rank[root_b]) {
            parent[root_a] = root_b;
        } 
        else {
            parent[root_b] = root_a;
            if (rank[root_a] == rank[root_b]) {
                rank[root_a]++;
            }
        }
    }

    bool connected(int a, int b) {
        return find(a) == find(b);
    }
};

class Percolation {
private:
    int n;
    std::vector<bool> grid;
    int open_count;
    UnionFind uf;
    int virtual_top;
    int virtual_bottom;

    int index(int row, int col) const {
        return row * n + col;
    }

public:
    Percolation(int size) : uf(size * size + 2) {
        n = size;
        open_count = 0;
        virtual_top    = n * n;
        virtual_bottom = n * n + 1;
        grid.resize(n * n, false);
    }

    int get_open_count() const {
        return open_count;
    }

    void open(int row, int col) {
        int i = index(row, col);
        if (grid[i]) return;
        
        grid[i] = true;
        open_count++;

        if (row == 0) uf.unite(i, virtual_top);
        
        if (row == n - 1) uf.unite(i, virtual_bottom);

        if (row - 1 >= 0 && grid[index(row - 1, col)]) uf.unite(i, index(row - 1, col));
        if (row + 1 <  n && grid[index(row + 1, col)]) uf.unite(i, index(row + 1, col));
        if (col - 1 >= 0 && grid[index(row, col - 1)]) uf.unite(i, index(row, col - 1));
        if (col + 1 <  n && grid[index(row, col + 1)]) uf.unite(i, index(row, col + 1));
    }

    bool percolates() {
        return uf.connected(virtual_top, virtual_bottom);
    }
};

struct PercolationStats {
private:
    size_t dim;
    size_t trials;
    std::vector<double> results;

    double run_one_experiment() {
        Percolation perc(dim);
        int total = dim * dim;
        
        std::vector<int> cells(total);
        for (int i = 0; i < total; i++) {
            cells[i] = i;
        }

        for (int i = total - 1; i > 0; i--) {
            int j = rand() % (i + 1);
            std::swap(cells[i], cells[j]);
        }

        for (int k = 0; k < total; k++) {
            int row = cells[k] / dim;
            int col = cells[k] % dim;
            
            perc.open(row, col);
            
            if (perc.percolates()) {
                return (double)perc.get_open_count() / total;
            }
        }
        return 1.0;
    }

public:
    PercolationStats(size_t dimension, size_t t) {
        dim    = dimension;
        trials = t;
    }

    double get_mean() const {
        double sum = 0.0;
        for (size_t t = 0; t < trials; t++) {
            sum += results[t];
        }
        return sum / trials;
    }

    double get_standard_deviation() const {
        double mean = get_mean();
        double sq_sum = 0.0;
        for (size_t t = 0; t < trials; t++) {
            double diff = results[t] - mean;
            sq_sum += diff * diff;
        }
        return sqrt(sq_sum / (trials - 1));
    }

    double get_confidence_low() const {
        return get_mean() - 1.96 * get_standard_deviation() / sqrt(trials);
    }

    double get_confidence_high() const {
        return get_mean() + 1.96 * get_standard_deviation() / sqrt(trials);
    }

    void execute() {
        srand(time(0));
        results.resize(trials);
        for (size_t t = 0; t < trials; t++) {
            results[t] = run_one_experiment();
        }
    }
};

int main() {
    size_t dimension = 100;
    size_t trials = 100;

    std::cout << "Grid: " << dimension << "x" << dimension
              << ", trials: " << trials << "\n\n";

    PercolationStats stats(dimension, trials);
    stats.execute();

    std::cout << "Mean:           " << stats.get_mean()               << "\n";
    std::cout << "Standard deviation:    " << stats.get_standard_deviation() << "\n";
    std::cout << "Confidence low:    " << stats.get_confidence_low()     << "\n";
    std::cout << "Confidence high:   " << stats.get_confidence_high()    << "\n";

    return 0;
}
