#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <cmath>
#include <stdexcept>
#include <iomanip>

class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank_;

public:
    explicit UnionFind(int n) : parent(n), rank_(n, 0) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);
        return parent[x];
    }

    void unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return;


        if (rank_[px] < rank_[py]) std::swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
    }

    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};


class Percolation {
private:
    size_t n;
    std::vector<bool> is_open_;
    UnionFind uf;
    int virtual_top;
    int virtual_bottom;
    size_t open_count_;

    int idx(int row, int col) const {
        return static_cast<int>(row * n + col);
    }

public:
    explicit Percolation(size_t dimension)
        : n(dimension),
          is_open_(dimension * dimension, false),
          uf(static_cast<int>(dimension * dimension + 2)),
          virtual_top(static_cast<int>(dimension * dimension)),
          virtual_bottom(static_cast<int>(dimension * dimension + 1)),
          open_count_(0)
    {
        if (dimension == 0)
            throw std::invalid_argument("Размер решётки должен быть > 0");
    }

    void open_site(int row, int col) {
        if (row < 0 || row >= static_cast<int>(n) ||
            col < 0 || col >= static_cast<int>(n))
            throw std::out_of_range("Индекс клетки выходит за пределы решётки");

        if (is_open_[idx(row, col)]) return;

        is_open_[idx(row, col)] = true;
        open_count_++;

        int cell = idx(row, col);


        if (row == 0)                    uf.unite(cell, virtual_top);
        if (row == static_cast<int>(n) - 1) uf.unite(cell, virtual_bottom);


        const int dr[] = {-1, +1,  0,  0};
        const int dc[] = { 0,  0, -1, +1};
        for (int d = 0; d < 4; d++) {
            int nr = row + dr[d];
            int nc = col + dc[d];
            if (nr >= 0 && nr < static_cast<int>(n) &&
                nc >= 0 && nc < static_cast<int>(n) &&
                is_open_[idx(nr, nc)])
            {
                uf.unite(cell, idx(nr, nc));
            }
        }
    }

    bool is_open(int row, int col) {
        return is_open_[idx(row, col)];
    }

    bool is_full(int row, int col) {
        return is_open_[idx(row, col)] &&
               uf.connected(idx(row, col), virtual_top);
    }

    bool percolates() {
        return uf.connected(virtual_top, virtual_bottom);
    }

    size_t number_of_open_sites() const {
        return open_count_;
    }

    size_t size() const { return n; }
};


struct PercolationStats {
private:
    size_t dimension_;
    size_t trials_;
    std::vector<double> thresholds_;
    double mean_;
    double stddev_;

public:
    PercolationStats(size_t dimension, size_t trials)
        : dimension_(dimension), trials_(trials), mean_(0.0), stddev_(0.0)
    {
        if (dimension == 0)
            throw std::invalid_argument("Размер решётки должен быть > 0");
        if (trials == 0)
            throw std::invalid_argument("Количество экспериментов должно быть > 0");
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
        std::mt19937 rng(std::random_device{}());
        size_t total_cells = dimension_ * dimension_;

        thresholds_.clear();
        thresholds_.reserve(trials_);

        for (size_t t = 0; t < trials_; t++) {
            Percolation perc(dimension_);

            std::vector<int> cells(total_cells);
            std::iota(cells.begin(), cells.end(), 0);
            std::shuffle(cells.begin(), cells.end(), rng);

            size_t opened = 0;
            for (int cell : cells) {
                int row = cell / static_cast<int>(dimension_);
                int col = cell % static_cast<int>(dimension_);
                perc.open_site(row, col);
                opened++;
                if (perc.percolates()) break;
            }

            thresholds_.push_back(static_cast<double>(opened) /
                                  static_cast<double>(total_cells));
        }

        double sum = 0.0;
        for (double x : thresholds_) sum += x;
        mean_ = sum / static_cast<double>(trials_);

        if (trials_ > 1) {
            double sq_sum = 0.0;
            for (double x : thresholds_) {
                double diff = x - mean_;
                sq_sum += diff * diff;
            }
            stddev_ = std::sqrt(sq_sum / static_cast<double>(trials_ - 1));
        } else {
            stddev_ = 0.0;
        }
    }
};

int main() {
    {
        size_t n = 20, T = 200;
        std::cout << "Решётка " << n << "×" << n << ", экспериментов: " << T << "\n";
        PercolationStats stats(n, T);
        stats.execute();
        std::cout << "  Среднее (x̄):           " << stats.get_mean() << "\n";
        std::cout << "  Стд. отклонение (s):    " << stats.get_standard_deviation() << "\n";
        std::cout << "  95%% дов. интервал: ["
                  << stats.get_confidence_low() << ", "
                  << stats.get_confidence_high() << "]\n\n";
    }

    {
        size_t n = 100, T = 100;
        std::cout << "Решётка " << n << "×" << n << ", экспериментов: " << T << "\n";
        PercolationStats stats(n, T);
        stats.execute();
        std::cout << "  Среднее (x̄):           " << stats.get_mean() << "\n";
        std::cout << "  Стд. отклонение (s):    " << stats.get_standard_deviation() << "\n";
        std::cout << "  95%% дов. интервал: ["
                  << stats.get_confidence_low() << ", "
                  << stats.get_confidence_high() << "]\n\n";
    }

    {
        std::cout << "Проверка корректности (решётка 3×3):\n";
        Percolation perc(3);
        std::cout << "  Протекает?  " << (perc.percolates() ? "да" : "нет") << " (ожид: нет)\n";
        perc.open_site(0, 0);
        perc.open_site(1, 0);
        perc.open_site(2, 0);
        std::cout << "  Протекает?  " << (perc.percolates() ? "да" : "нет") << " (ожид: да)\n";
        std::cout << "  Открыто:    " << perc.number_of_open_sites() << " (ожид: 3)\n";
        std::cout << "  Полная [1][0]: " << (perc.is_full(1, 0) ? "да" : "нет") << " (ожид: да)\n";
        std::cout << "  Полная [1][2]: " << (perc.is_full(1, 2) ? "да" : "нет") << " (ожид: нет)\n";
    }
    return 0;
}