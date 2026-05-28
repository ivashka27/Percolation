#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <stdexcept>

// UnionFind
class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return;
        if (rank[rx] < rank[ry])
            parent[rx] = ry;
        else if (rank[rx] > rank[ry])
            parent[ry] = rx;
        else {
            parent[ry] = rx;
            rank[rx]++;
        }
    }

    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};

// Percolation
class Percolation {
private:
    int n;
    std::vector<std::vector<bool>> grid;
    UnionFind uf;
    int virtualTop;
    int virtualBottom;

    int index(int row, int col) const {
        return row * n + col;
    }

    void validate(int row, int col) const {
        if (row < 0 || row >= n || col < 0 || col >= n)
            throw std::out_of_range("row or col out of bounds");
    }

public:
    Percolation(int size) : n(size), grid(size, std::vector<bool>(size, false)),
                            uf(size * size + 2),
                            virtualTop(size * size),
                            virtualBottom(size * size + 1) {}

    void open(int row, int col) {
        validate(row, col);
        if (grid[row][col]) return;

        grid[row][col] = true;
        int id = index(row, col);

        if (row == 0)
            uf.unite(id, virtualTop);
        if (row == n - 1)
            uf.unite(id, virtualBottom);

        const int dr[] = {-1, 1, 0, 0};
        const int dc[] = {0, 0, -1, 1};
        for (int d = 0; d < 4; ++d) {
            int nr = row + dr[d];
            int nc = col + dc[d];
            if (nr >= 0 && nr < n && nc >= 0 && nc < n && grid[nr][nc])
                uf.unite(id, index(nr, nc));
        }
    }

    bool is_open(int row, int col) const {
        validate(row, col);
        return grid[row][col];
    }

    bool percolates() {
        return uf.connected(virtualTop, virtualBottom);
    }
};

// PercolationStats
class PercolationStats {
private:
    size_t dimension;
    size_t trials;
    std::vector<double> thresholds;
    double mean_val;
    double stddev_val;
    double conf_low;
    double conf_high;

    static std::mt19937& rng() {
        static std::mt19937 gen(std::random_device{}());
        return gen;
    }

public:
    PercolationStats(size_t dim, size_t t) : dimension(dim), trials(t), thresholds(t) {
        if (dim <= 0 || trials <= 0)
            throw std::invalid_argument("dimension and trials must be positive");
    }

    void execute() {
        size_t total_cells = dimension * dimension;

        for (size_t exp = 0; exp < trials; ++exp) {
            Percolation perc(static_cast<int>(dimension));

            std::vector<int> cells(total_cells);
            for (size_t i = 0; i < total_cells; ++i)
                cells[i] = static_cast<int>(i);
            std::shuffle(cells.begin(), cells.end(), rng());

            int opened = 0;
            for (int idx : cells) {
                int row = idx / static_cast<int>(dimension);
                int col = idx % static_cast<int>(dimension);
                perc.open(row, col);
                ++opened;
                if (perc.percolates())
                    break;
            }
            thresholds[exp] = static_cast<double>(opened) / total_cells;
        }

        double sum = 0.0;
        for (double t : thresholds) sum += t;
        mean_val = sum / trials;

        double sum_sq = 0.0;
        for (double t : thresholds) {
            double diff = t - mean_val;
            sum_sq += diff * diff;
        }
        stddev_val = std::sqrt(sum_sq / (trials - 1));

        double half_width = 1.96 * stddev_val / std::sqrt(trials);
        conf_low = mean_val - half_width;
        conf_high = mean_val + half_width;
    }

    double get_mean() const { return mean_val; }
    double get_standard_deviation() const { return stddev_val; }
    double get_confidence_low() const { return conf_low; }
    double get_confidence_high() const { return conf_high; }
};


int main(int argc, char* argv[]) {
    size_t n = 200;
    size_t T = 100;
    if (argc >= 3) {
        n = static_cast<size_t>(std::stoi(argv[1]));
        T = static_cast<size_t>(std::stoi(argv[2]));
    }

    std::cout << "Dimension: " << n << " x " << n << ", Trials: " << T << std::endl;

    PercolationStats stats(n, T);
    stats.execute();

    std::cout << "Mean                    = " << stats.get_mean() << std::endl;
    std::cout << "Standard deviation      = " << stats.get_standard_deviation() << std::endl;
    std::cout << "95% confidence interval = ["
              << stats.get_confidence_low() << ", "
              << stats.get_confidence_high() << "]" << std::endl;

    return 0;
}