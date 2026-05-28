#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <stdexcept>
#include <vector>

class Percolation {
public:
    explicit Percolation(size_t n)
        : n_(n), grid_(n * n, false),
          parent_(n * n + 2), rank_(n * n + 2, 0)
    {
        if (n == 0) throw std::invalid_argument("Grid dimension must be > 0");
        std::iota(parent_.begin(), parent_.end(), size_t{0});
    }

    void open(size_t row, size_t col) {
        if (row >= n_ || col >= n_) throw std::out_of_range("Cell out of range");
        if (grid_[idx(row, col)]) return;

        grid_[idx(row, col)] = true;
        ++open_count_;

        size_t i = idx(row, col);

        if (row == 0)      unite(i, virtual_top());
        if (row == n_ - 1) unite(i, virtual_bottom());

        auto try_unite = [&](size_t r, size_t c) {
            if (r < n_ && c < n_ && grid_[idx(r, c)])
                unite(i, idx(r, c));
        };
        if (row > 0)      try_unite(row - 1, col);
        if (row + 1 < n_) try_unite(row + 1, col);
        if (col > 0)      try_unite(row, col - 1);
        if (col + 1 < n_) try_unite(row, col + 1);
    }

    bool is_open(size_t row, size_t col) const {
        return grid_[idx(row, col)];
    }

    bool is_full(size_t row, size_t col) const {
        if (!grid_[idx(row, col)]) return false;
        return connected(idx(row, col), virtual_top());
    }

    size_t number_of_open_sites() const { return open_count_; }

    bool percolates() const {
        return connected(virtual_top(), virtual_bottom());
    }

private:
    size_t n_;
    size_t open_count_ = 0;
    std::vector<bool>   grid_;
    std::vector<size_t> parent_;
    std::vector<size_t> rank_;

    size_t virtual_top()    const { return n_ * n_; }
    size_t virtual_bottom() const { return n_ * n_ + 1; }
    size_t idx(size_t r, size_t c) const { return r * n_ + c; }

    size_t find(size_t x) {
        while (parent_[x] != x) {
            parent_[x] = parent_[parent_[x]]; 
            x = parent_[x];
        }
        return x;
    }

    void unite(size_t a, size_t b) {
        size_t ra = find(a), rb = find(b);
        if (ra == rb) return;
        if (rank_[ra] < rank_[rb]) std::swap(ra, rb);
        parent_[rb] = ra;
        if (rank_[ra] == rank_[rb]) ++rank_[ra];
    }

    bool connected(size_t a, size_t b) const {
        return const_cast<Percolation*>(this)->find(a) ==
               const_cast<Percolation*>(this)->find(b);
    }
};


struct PercolationStats {
    PercolationStats(size_t dimension, size_t trials)
        : dimension_(dimension), trials_(trials)
    {
        if (dimension == 0) throw std::invalid_argument("Dimension must be > 0");
        if (trials   == 0) throw std::invalid_argument("Trials must be > 0");
    }

    double get_mean() const { return mean_; }

    double get_standard_deviation() const { return stddev_; }

    double get_confidence_low() const {
        return mean_ - 1.96 * stddev_ / std::sqrt(static_cast<double>(trials_));
    }

    double get_confidence_high() const {
        return mean_ + 1.96 * stddev_ / std::sqrt(static_cast<double>(trials_));
    }

    void execute() {
        thresholds_.resize(trials_);
        for (size_t t = 0; t < trials_; ++t)
            thresholds_[t] = run_experiment();

        double sum = std::accumulate(thresholds_.begin(), thresholds_.end(), 0.0);
        mean_ = sum / static_cast<double>(trials_);

        double sq = 0.0;
        for (double x : thresholds_) { double d = x - mean_; sq += d * d; }
        stddev_ = (trials_ > 1)
            ? std::sqrt(sq / static_cast<double>(trials_ - 1))
            : 0.0;
    }

private:
    size_t dimension_, trials_;
    std::vector<double> thresholds_;
    double mean_ = 0.0, stddev_ = 0.0;

    double run_experiment() const {
        const size_t n = dimension_;
        Percolation perc(n);

        std::vector<size_t> cells(n * n);
        std::iota(cells.begin(), cells.end(), size_t{0});
        std::mt19937 rng(std::random_device{}());
        std::shuffle(cells.begin(), cells.end(), rng);

        for (size_t k : cells) {
            perc.open(k / n, k % n);
            if (perc.percolates()) break;
        }
        return static_cast<double>(perc.number_of_open_sites()) / (n * n);
    }
};


int main(int argc, char* argv[]) {
    size_t n = 200, T = 100;
    if (argc == 3) {
        n = static_cast<size_t>(std::stoul(argv[1]));
        T = static_cast<size_t>(std::stoul(argv[2]));
    }

    std::cout << "Grid: " << n << "x" << n << "  Trials: " << T << "\n";

    PercolationStats stats(n, T);
    stats.execute();

    std::cout << std::fixed << std::setprecision(6)
              << "mean                    = " << stats.get_mean()               << "\n"
              << "stddev                  = " << stats.get_standard_deviation() << "\n"
              << "95% confidence interval = ["
              << stats.get_confidence_low()  << ", "
              << stats.get_confidence_high() << "]\n";
}
