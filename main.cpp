#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <numeric>
#include <stdexcept>
#include <iomanip>

class DSU {
    std::vector<size_t> parent;
    std::vector<size_t> rank;

public:
    DSU(size_t n) : parent(n), rank(n, 0) {
        for (size_t i = 0; i < n; ++i) parent[i] = i;
    }

    size_t find(size_t x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(size_t a, size_t b) {
        a = find(a);
        b = find(b);
        if (a == b) return;
        if (rank[a] < rank[b]) std::swap(a, b);
        parent[b] = a;
        if (rank[a] == rank[b]) ++rank[a];
    }

    bool connected(size_t a, size_t b) {
        return find(a) == find(b);
    }
};

class Percolation {
    size_t n;
    size_t opened_cnt = 0;
    size_t VTOP, VBOT;
    std::vector<char> grid;
    DSU dsu;

    size_t idx(size_t r, size_t c) const { return r * n + c; }

public:
    Percolation(size_t dim) 
        : n(dim), VTOP(dim * dim), VBOT(dim * dim + 1),
          grid(dim * dim, 0), dsu(dim * dim + 2) {
        if (dim == 0) throw std::invalid_argument("Grid size must be > 0");
    }

    void open(size_t r, size_t c) {
        if (r >= n || c >= n) throw std::out_of_range("Coordinates out of bounds");
        size_t i = idx(r, c);
        if (grid[i]) return;

        grid[i] = 1;
        ++opened_cnt;

        if (r == 0) dsu.unite(i, VTOP);
        if (r == n - 1) dsu.unite(i, VBOT);

        if (r > 0 && grid[idx(r - 1, c)]) dsu.unite(i, idx(r - 1, c));
        if (r + 1 < n && grid[idx(r + 1, c)]) dsu.unite(i, idx(r + 1, c));
        if (c > 0 && grid[idx(r, c - 1)]) dsu.unite(i, idx(r, c - 1));
        if (c + 1 < n && grid[idx(r, c + 1)]) dsu.unite(i, idx(r, c + 1));
    }

    bool percolates() { return dsu.connected(VTOP, VBOT); }
    size_t opened() const { return opened_cnt; }
    size_t dimension() const { return n; }
};

struct PercolationStats {
    size_t n, trials;
    double mean_val = 0, stddev_val = 0, conf_lo = 0, conf_hi = 0;
    std::mt19937 rng;
    std::vector<size_t> pool;

    PercolationStats(size_t dim, size_t t)
        : n(dim), trials(t), rng(std::random_device{}()), pool(dim * dim) {
        if (dim == 0) throw std::invalid_argument("Dimension must be positive");
        if (t < 2) throw std::invalid_argument("Need at least 2 trials");
    }

    double run_one() {
        Percolation p(n);
        for (size_t i = 0; i < n * n; ++i) pool[i] = i;
        size_t remaining = n * n;

        while (!p.percolates()) {
            size_t pick = std::uniform_int_distribution<size_t>{0, remaining - 1}(rng);
            size_t cell = pool[pick];
            p.open(cell / n, cell % n);
            pool[pick] = pool[--remaining];
        }
        return static_cast<double>(p.opened()) / (n * n);
    }

    void execute() {
        double sum = 0, sq_sum = 0;
        for (size_t i = 0; i < trials; ++i) {
            double x = run_one();
            sum += x;
            sq_sum += x * x;
        }
        mean_val = sum / trials;
        double var = (sq_sum - sum * sum / trials) / (trials - 1);
        if (var < 0) var = 0;
        stddev_val = std::sqrt(var);
        double m = 1.96 * stddev_val / std::sqrt(static_cast<double>(trials));
        conf_lo = mean_val - m;
        conf_hi = mean_val + m;
    }

    double mean() const { return mean_val; }
    double stddev() const { return stddev_val; }
    double confidenceLow() const { return conf_lo; }
    double confidenceHigh() const { return conf_hi; }
};

int main() {
    try {
        size_t n, t;
        std::cin >> n >> t;
        PercolationStats ps(n, t);
        ps.execute();
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "mean                    = " << ps.mean() << '\n';
        std::cout << "stddev                  = " << ps.stddev() << '\n';
        std::cout << "95% confidence interval = ["
                  << ps.confidenceLow() << ", " << ps.confidenceHigh() << "]\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}