#include <vector>
#include <cmath>
#include <random>
#include <stdexcept>

// Класс для моделирования самой решетки с помощью DSU (Union-Find)
class Percolation {
private:
    size_t size;
    std::vector<bool> open_sites;
    std::vector<size_t> parent;
    std::vector<size_t> rank;
    size_t open_count;
    
    size_t virtual_top;
    size_t virtual_bottom;

    size_t get_index(size_t row, size_t col) const {
        return row * size + col;
    }

    size_t find(size_t i) {
        if (parent[i] == i) return i;
        // Эвристика сжатия пути для ускорения до O(α(N))
        return parent[i] = find(parent[i]);
    }

    void unite(size_t i, size_t j) {
        size_t root_i = find(i);
        size_t root_j = find(j);
        if (root_i != root_j) {
            // Эвристика объединения по рангу
            if (rank[root_i] < rank[root_j]) {
                parent[root_i] = root_j;
            } else if (rank[root_i] > rank[root_j]) {
                parent[root_j] = root_i;
            } else {
                parent[root_j] = root_i;
                rank[root_i]++;
            }
        }
    }

public:
    Percolation(size_t n) : size(n), open_sites(n * n, false), open_count(0) {
        virtual_top = n * n;
        virtual_bottom = n * n + 1;
        
        parent.resize(n * n + 2);
        rank.resize(n * n + 2, 0);
        
        for (size_t i = 0; i < parent.size(); ++i) {
            parent[i] = i;
        }
    }

    void open(size_t row, size_t col) {
        size_t idx = get_index(row, col);
        if (open_sites[idx]) return;
        
        open_sites[idx] = true;
        open_count++;

        // Если ячейка в верхнем или нижнем ряду, соединяем с виртуальными вершинами
        if (row == 0) unite(idx, virtual_top);
        if (row == size - 1) unite(idx, virtual_bottom);

        // Объединяем с открытыми соседями (верх, низ, лево, право)
        if (row > 0 && open_sites[get_index(row - 1, col)]) unite(idx, get_index(row - 1, col));
        if (row < size - 1 && open_sites[get_index(row + 1, col)]) unite(idx, get_index(row + 1, col));
        if (col > 0 && open_sites[get_index(row, col - 1)]) unite(idx, get_index(row, col - 1));
        if (col < size - 1 && open_sites[get_index(row, col + 1)]) unite(idx, get_index(row, col + 1));
    }

    bool is_open(size_t row, size_t col) const {
        return open_sites[get_index(row, col)];
    }

    bool percolates() {
        if (size == 0) return false;
        return find(virtual_top) == find(virtual_bottom);
    }

    size_t get_open_count() const {
        return open_count;
    }
};

// Структура по заданию из инструкции
struct PercolationStats {
    size_t dimension;
    size_t trials;
    
    double mean_val;
    double std_dev_val;

    /**
     * Construct a new Percolation Stats object
     * @param dimension dimension of percolation grid
     * @param trials amount of experiments
     */
    PercolationStats(size_t dimension, size_t trials) 
        : dimension(dimension), trials(trials), mean_val(0.0), std_dev_val(0.0) {
        if (dimension == 0 || trials == 0) {
            throw std::invalid_argument("Dimension and trials must be > 0");
        }
    }

    /**
     * Makes all experiments, calculates statistic values
     */
    void execute() {
        std::vector<double> thresholds;
        thresholds.reserve(trials);
        
        // Настройка генератора случайных чисел с равномерным распределением
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dist(0, dimension - 1);

        double sum = 0.0;

        for (size_t t = 0; t < trials; ++t) {
            Percolation perc(dimension);
            
            // Пока система не протекает, случайным образом открываем клетки
            while (!perc.percolates()) {
                size_t r = dist(gen);
                size_t c = dist(gen);
                perc.open(r, c); // Метод open внутри сам проверяет, была ли клетка уже открыта
            }
            
            double threshold = static_cast<double>(perc.get_open_count()) / (dimension * dimension);
            thresholds.push_back(threshold);
            sum += threshold;
        }

        // Вычисление выборочного среднего x̄
        mean_val = sum / trials;

        // Вычисление дисперсии s^2 и стандартного отклонения s
        if (trials > 1) {
            double sum_sq_diff = 0.0;
            for (double val : thresholds) {
                sum_sq_diff += (val - mean_val) * (val - mean_val);
            }
            std_dev_val = std::sqrt(sum_sq_diff / (trials - 1));
        } else {
            std_dev_val = 0.0; // Защита от деления на ноль при T=1
        }
    }

    /**
     * Returns mean of percolation threshold
     */
    double get_mean() const {
        return mean_val;
    }

    /**
     * Returns standard deviation of percolation threshold
     */
    double get_standard_deviation() const {
        return std_dev_val;
    }

    /**
     * Returns log edge of confidence interval (нижняя граница)
     */
    double get_confidence_low() const {
        return mean_val - (1.96 * std_dev_val) / std::sqrt(trials);
    }

    /**
     * Returns high edge of confidence interval (верхняя граница)
     */
    double get_confidence_high() const {
        return mean_val + (1.96 * std_dev_val) / std::sqrt(trials);
    }
};
