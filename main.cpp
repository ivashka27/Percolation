#include <vector>
#include <random>
#include <cmath>
#include <numeric>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <iomanip>


class Percolation {
private:
    int n;
    std::vector<bool> open_sites;
    std::vector<int> parent;
    std::vector<int> size;
    int virtual_top;
    int virtual_bottom;
    int open_count;

    int index(int row, int col) const {
        return row * n + col;
    }

    bool is_valid(int row, int col) const {
        return row >= 0 && row < n && col >= 0 && col < n;
    }

    int find(int p) {
        while (p != parent[p]) {
            parent[p] = parent[parent[p]];
            p = parent[p];
        }
        return p;
    }

    void union_sets(int p, int q) {
        int root_p = find(p);
        int root_q = find(q);
        if (root_p == root_q) return;

        if (size[root_p] < size[root_q]) {
            parent[root_p] = root_q;
            size[root_q] += size[root_p];
        }
        else {
            parent[root_q] = root_p;
            size[root_p] += size[root_q];
        }
    }

public:
    Percolation(int n) : n(n), open_count(0) {
        if (n <= 0) {
            throw std::invalid_argument("Dimension must be positive");
        }

        open_sites.assign(n * n, false);
        parent.resize(n * n + 2);
        size.resize(n * n + 2, 1);

        for (int i = 0; i < n * n + 2; i++) {
            parent[i] = i;
        }

        virtual_top = n * n;
        virtual_bottom = n * n + 1;
    }

    void open(int row, int col) {
        if (!is_valid(row, col)) {
            throw std::out_of_range("Invalid cell coordinates");
        }

        if (open_sites[index(row, col)]) return;

        open_sites[index(row, col)] = true;
        open_count++;

        int idx = index(row, col);

        if (row == 0) {
            union_sets(idx, virtual_top);
        }
        if (row == n - 1) {
            union_sets(idx, virtual_bottom);
        }

        int dr[] = { -1, 1, 0, 0 };
        int dc[] = { 0, 0, -1, 1 };

        for (int i = 0; i < 4; i++) {
            int nr = row + dr[i];
            int nc = col + dc[i];
            if (is_valid(nr, nc) && is_open(nr, nc)) {
                union_sets(idx, index(nr, nc));
            }
        }
    }

    bool is_open(int row, int col) const {
        if (!is_valid(row, col)) {
            throw std::out_of_range("Invalid cell coordinates");
        }
        return open_sites[index(row, col)];
    }

    bool is_full(int row, int col) {
        if (!is_valid(row, col)) {
            throw std::out_of_range("Invalid cell coordinates");
        }
        if (!is_open(row, col)) return false;
        return find(index(row, col)) == find(virtual_top);
    }

    int number_of_open_sites() const {
        return open_count;
    }

    bool percolates() {
        return find(virtual_top) == find(virtual_bottom);
    }
};

struct PercolationStats {
private:
    size_t dimension;
    size_t trials;
    double mean;
    double std_dev;
    double confidence_low;
    double confidence_high;

public:
    /**
     * Construct a new Percolation Stats object
     * @param dimension dimension of percolation grid
     * @param trials amount of experiments
     */
    PercolationStats(size_t dimension, size_t trials)
        : dimension(dimension), trials(trials),
        mean(0.0), std_dev(0.0),
        confidence_low(0.0), confidence_high(0.0) {
        if (dimension == 0 || trials == 0) {
            throw std::invalid_argument("Dimension and trials must be positive");
        }
    }

    /**
     * Makes all experiments, calculates statistic values
     */
    void execute() {
        std::random_device rd;
        std::mt19937 gen(rd());

        std::vector<double> thresholds;
        thresholds.reserve(trials);

        for (size_t t = 0; t < trials; t++) {
            Percolation perc(static_cast<int>(dimension));

            std::vector<int> blocked;
            for (int i = 0; i < static_cast<int>(dimension); i++) {
                for (int j = 0; j < static_cast<int>(dimension); j++) {
                    blocked.push_back(i * static_cast<int>(dimension) + j);
                }
            }

            std::shuffle(blocked.begin(), blocked.end(), gen);

            int open_count = 0;

            for (size_t i = 0; i < blocked.size() && !perc.percolates(); i++) {
                int pos = blocked[i];
                int row = pos / static_cast<int>(dimension);
                int col = pos % static_cast<int>(dimension);
                perc.open(row, col);
                open_count++;
            }

            double x_t = static_cast<double>(open_count) /
                static_cast<double>(dimension * dimension);
            thresholds.push_back(x_t);
        }

        mean = std::accumulate(thresholds.begin(), thresholds.end(), 0.0) /
            static_cast<double>(trials);

        double sum_sq_diff = 0.0;
        for (double x_t : thresholds) {
            double diff = x_t - mean;
            sum_sq_diff += diff * diff;
        }

        if (trials > 1) {
            std_dev = std::sqrt(sum_sq_diff / static_cast<double>(trials - 1));
        }
        else {
            std_dev = 0.0;
        }

        const double z = 1.96;
        double margin = z * std_dev / std::sqrt(static_cast<double>(trials));
        confidence_low = mean - margin;
        confidence_high = mean + margin;
    }

    /**
     * Returns mean of percolation threshold (x¯ from description)
     */
    double get_mean() const {
        return mean;
    }

    /**
     * Returns standard deviation of percolation threshold (s from description)
     */
    double get_standard_deviation() const {
        return std_dev;
    }

    /**
     * Returns low edge of confidence interval
     */
    double get_confidence_low() const {
        return confidence_low;
    }

    /**
     * Returns high edge of confidence interval
     */
    double get_confidence_high() const {
        return confidence_high;
    }
};


int main() {
    try {

        PercolationStats stats(200, 100);

        stats.execute();

        // results
        std::cout << std::fixed << std::setprecision(6);
        std::cout << "Mean percolation threshold = " << stats.get_mean() << std::endl;
        std::cout << "Standard deviation = " << stats.get_standard_deviation() << std::endl;
        std::cout << "95% confidence interval = ["
            << stats.get_confidence_low() << ", "
            << stats.get_confidence_high() << "]" << std::endl;

        return 0;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
