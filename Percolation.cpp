#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>
#include <stdexcept>
#include <vector>

class DSU {
public:
    explicit DSU(size_t size)
        : parent_(size), size_(size, 1)
    {
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    size_t find(size_t v) {
        if (parent_[v] == v) {
            return v;
        }

        parent_[v] = find(parent_[v]);
        return parent_[v];
    }

    void unite(size_t a, size_t b) {
        a = find(a);
        b = find(b);

        if (a == b) {
            return;
        }

        if (size_[a] < size_[b]) {
            std::swap(a, b);
        }

        parent_[b] = a;
        size_[a] += size_[b];
    }

    bool connected(size_t a, size_t b) const {
        return root(a) == root(b);
    }

private:
    size_t root(size_t v) const {
        while (parent_[v] != v) {
            v = parent_[v];
        }

        return v;
    }

private:
    std::vector<size_t> parent_;
    std::vector<size_t> size_;
};

class Percolation {
public:
    explicit Percolation(size_t dimension)
        : dimension_(dimension),
        open_(dimension* dimension, false),
        open_count_(0),
        virtual_top_(dimension* dimension),
        virtual_bottom_(dimension* dimension + 1),
        percolation_dsu_(dimension* dimension + 2),
        fullness_dsu_(dimension* dimension + 1)
    {
        if (dimension == 0) {
            throw std::invalid_argument("dimension must be positive");
        }
    }

    void open(size_t row, size_t col) {
        validate(row, col);

        const size_t current = index(row, col);

        if (open_[current]) {
            return;
        }

        open_[current] = true;
        ++open_count_;

        if (row == 0) {
            percolation_dsu_.unite(current, virtual_top_);
            fullness_dsu_.unite(current, virtual_top_);
        }

        if (row + 1 == dimension_) {
            percolation_dsu_.unite(current, virtual_bottom_);
        }

        connect_if_open(row, col, row > 0, row - 1, col);
        connect_if_open(row, col, row + 1 < dimension_, row + 1, col);
        connect_if_open(row, col, col > 0, row, col - 1);
        connect_if_open(row, col, col + 1 < dimension_, row, col + 1);
    }

    bool is_open(size_t row, size_t col) const {
        validate(row, col);
        return open_[index(row, col)];
    }

    bool is_full(size_t row, size_t col) const {
        validate(row, col);

        const size_t current = index(row, col);

        return open_[current] && fullness_dsu_.connected(current, virtual_top_);
    }

    bool percolates() const {
        return percolation_dsu_.connected(virtual_top_, virtual_bottom_);
    }

    size_t get_open_count() const {
        return open_count_;
    }

    size_t get_dimension() const {
        return dimension_;
    }

private:
    size_t index(size_t row, size_t col) const {
        return row * dimension_ + col;
    }

    void validate(size_t row, size_t col) const {
        if (row >= dimension_ || col >= dimension_) {
            throw std::out_of_range("cell index is out of range");
        }
    }

    void connect_if_open(
        size_t row,
        size_t col,
        bool condition,
        size_t neighbor_row,
        size_t neighbor_col
    ) {
        if (!condition) {
            return;
        }

        if (!is_open(neighbor_row, neighbor_col)) {
            return;
        }

        const size_t current = index(row, col);
        const size_t neighbor = index(neighbor_row, neighbor_col);

        percolation_dsu_.unite(current, neighbor);
        fullness_dsu_.unite(current, neighbor);
    }

private:
    size_t dimension_;
    std::vector<bool> open_;
    size_t open_count_;

    size_t virtual_top_;
    size_t virtual_bottom_;

    DSU percolation_dsu_;
    DSU fullness_dsu_;
};

struct PercolationStats {
    PercolationStats(size_t dimension, size_t trials)
        : dimension_(dimension),
        trials_(trials),
        mean_(0.0),
        standard_deviation_(0.0),
        confidence_low_(0.0),
        confidence_high_(0.0),
        generator_(std::random_device{}())
    {
        if (dimension == 0) {
            throw std::invalid_argument("dimension must be positive");
        }

        if (trials == 0) {
            throw std::invalid_argument("trials must be positive");
        }
    }

    double get_mean() const {
        return mean_;
    }

    double get_standard_deviation() const {
        return standard_deviation_;
    }

    double get_confidence_low() const {
        return confidence_low_;
    }

    double get_confidence_high() const {
        return confidence_high_;
    }

    void execute() {
        thresholds_.clear();
        thresholds_.reserve(trials_);

        for (size_t trial = 0; trial < trials_; ++trial) {
            thresholds_.push_back(run_single_experiment());
        }

        calculate_statistics();
    }

private:
    double run_single_experiment() {
        Percolation percolation(dimension_);

        const size_t total_cells = dimension_ * dimension_;

        std::vector<size_t> cells(total_cells);
        std::iota(cells.begin(), cells.end(), 0);

        std::shuffle(cells.begin(), cells.end(), generator_);

        for (size_t cell : cells) {
            const size_t row = cell / dimension_;
            const size_t col = cell % dimension_;

            percolation.open(row, col);

            if (percolation.percolates()) {
                break;
            }
        }

        return static_cast<double>(percolation.get_open_count())
            / static_cast<double>(total_cells);
    }

    void calculate_statistics() {
        mean_ = 0.0;

        for (double value : thresholds_) {
            mean_ += value;
        }

        mean_ /= static_cast<double>(trials_);

        if (trials_ == 1) {
            standard_deviation_ = 0.0;
        }
        else {
            double variance_sum = 0.0;

            for (double value : thresholds_) {
                const double difference = value - mean_;
                variance_sum += difference * difference;
            }

            standard_deviation_ = std::sqrt(
                variance_sum / static_cast<double>(trials_ - 1)
            );
        }

        const double confidence_delta =
            1.96 * standard_deviation_ / std::sqrt(static_cast<double>(trials_));

        confidence_low_ = mean_ - confidence_delta;
        confidence_high_ = mean_ + confidence_delta;
    }

private:
    size_t dimension_;
    size_t trials_;

    std::vector<double> thresholds_;

    double mean_;
    double standard_deviation_;
    double confidence_low_;
    double confidence_high_;

    std::mt19937 generator_;
};

int main() {
    PercolationStats stats(100, 50);
    stats.execute();

    std::cout << "mean = " << stats.get_mean() << '\n';
    std::cout << "stddev = " << stats.get_standard_deviation() << '\n';
    std::cout << "95% confidence interval = ["
        << stats.get_confidence_low()
        << ", "
        << stats.get_confidence_high()
        << "]\n";

    return 0;
}