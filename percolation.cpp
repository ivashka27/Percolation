#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <algorithm>
#include <cmath>
#include <stdexcept>

class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> size;

public:
    explicit UnionFind(int n) {
        parent.resize(n);
        size.assign(n, 1);
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int v) {
        if (parent[v] == v) {
            return v;
        }

        parent[v] = find(parent[v]);
        return parent[v];
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a == b) {
            return;
        }

        if (size[a] < size[b]) {
            std::swap(a, b);
        }

        parent[b] = a;
        size[a] += size[b];
    }

    bool connected(int a, int b) {
        return find(a) == find(b);
    }
};

class Percolation {
private:
    size_t dimension;
    std::vector<bool> opened;
    UnionFind unionfind;

    int top_virtual;
    int bottom_virtual;
    size_t opened_count;

    int get_index(size_t row, size_t col) const {
        return static_cast<int>(row * dimension + col);
    }

    bool is_valid(size_t row, size_t col) const {
        return row < dimension && col < dimension;
    }

public:
    explicit Percolation(size_t n)
        : dimension(n),
          opened(n * n, false),
          unionfind(static_cast<int>(n * n + 2)),
          top_virtual(static_cast<int>(n * n)),
          bottom_virtual(static_cast<int>(n * n + 1)),
          opened_count(0)
    {
        if (n == 0) {
            throw std::invalid_argument("Dimension must be positive");
        }
    }

    void open(size_t row, size_t col) {
        if (!is_valid(row, col)) {
            throw std::out_of_range("Cell coordinates are out of range");
        }

        int current = get_index(row, col);

        if (opened[current]) {
            return;
        }

        opened[current] = true;
        ++opened_count;

        if (row == 0) {
            unionfind.unite(current, top_virtual);
        }

        if (row == dimension - 1) {
            unionfind.unite(current, bottom_virtual);
        }

        const int drow[4] = {-1, 1, 0, 0};
        const int dcol[4] = {0, 0, -1, 1};

        for (int k = 0; k < 4; ++k) {
            int new_row = static_cast<int>(row) + drow[k];
            int new_col = static_cast<int>(col) + dcol[k];

            if (new_row < 0 || new_col < 0) {
                continue;
            }

            size_t nr = static_cast<size_t>(new_row);
            size_t nc = static_cast<size_t>(new_col);

            if (!is_valid(nr, nc)) {
                continue;
            }

            int neighbor = get_index(nr, nc);

            if (opened[neighbor]) {
                unionfind.unite(current, neighbor);
            }
        }
    }

    bool is_open(size_t row, size_t col) const {
        if (!is_valid(row, col)) {
            throw std::out_of_range("Cell coordinates are out of range");
        }

        return opened[get_index(row, col)];
    }

    bool percolates() {
        return unionfind.connected(top_virtual, bottom_virtual);
    }

    size_t get_opened_count() const {
        return opened_count;
    }

    size_t get_dimension() const {
        return dimension;
    }
};

struct PercolationStats {
private:
    size_t dimension;
    size_t trials;

    std::vector<double> results;

    double mean = 0.0;
    double standard_deviation = 0.0;
    double confidence_low = 0.0;
    double confidence_high = 0.0;

public:
    PercolationStats(size_t dimension, size_t trials)
        : dimension(dimension), trials(trials)
    {
        if (dimension == 0) {
            throw std::invalid_argument("Dimension must be positive");
        }

        if (trials == 0) {
            throw std::invalid_argument("Trials must be positive");
        }
    }

    double get_mean() const {
        return mean;
    }

    double get_standard_deviation() const {
        return standard_deviation;
    }

    double get_confidence_low() const {
        return confidence_low;
    }

    double get_confidence_high() const {
        return confidence_high;
    }

    void execute() {
        results.clear();
        results.reserve(trials);

        std::random_device rd;
        std::mt19937 gen(rd());

        for (size_t experiment = 0; experiment < trials; ++experiment) {
            Percolation percolation(dimension);

            std::vector<int> cells(dimension * dimension);
            std::iota(cells.begin(), cells.end(), 0);

            std::shuffle(cells.begin(), cells.end(), gen);

            size_t current_cell = 0;

            while (!percolation.percolates()) {
                int cell = cells[current_cell++];

                size_t row = static_cast<size_t>(cell) / dimension;
                size_t col = static_cast<size_t>(cell) % dimension;

                percolation.open(row, col);
            }

            double threshold =
                static_cast<double>(percolation.get_opened_count()) /
                static_cast<double>(dimension * dimension);

            results.push_back(threshold);
        }

        calculate_statistics();
    }

private:
    void calculate_statistics() {
        double sum = std::accumulate(results.begin(), results.end(), 0.0);
        mean = sum / static_cast<double>(trials);

        if (trials == 1) {
            standard_deviation = 0.0;
        } else {
            double squared_sum = 0.0;

            for (double value : results) {
                squared_sum += (value - mean) * (value - mean);
            }

            standard_deviation = std::sqrt(
                squared_sum / static_cast<double>(trials - 1)
            );
        }

        double margin =
            1.96 * standard_deviation / std::sqrt(static_cast<double>(trials));

        confidence_low = mean - margin;
        confidence_high = mean + margin;
    }
};

int main() {
    size_t dimension;
    size_t trials;

    std::cout << "Enter grid dimension: ";
    std::cin >> dimension;

    std::cout << "Enter number of trials: ";
    std::cin >> trials;

    try {
        PercolationStats stats(dimension, trials);
        stats.execute();

        std::cout << "mean                    = "
                  << stats.get_mean() << '\n';

        std::cout << "stddev                  = "
                  << stats.get_standard_deviation() << '\n';

        std::cout << "95% confidence interval = ["
                  << stats.get_confidence_low()
                  << ", "
                  << stats.get_confidence_high()
                  << "]\n";
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}