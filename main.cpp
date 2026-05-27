#include <cmath>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <random>
#include <stdexcept>
#include <vector>

class DisjointSetUnion {
private:
    std::vector<size_t> parent_;
    std::vector<size_t> size_;

public:
    explicit DisjointSetUnion(size_t amount)
        : parent_(amount), size_(amount, 1)
    {
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    size_t find(size_t vertex)
    {
        if (parent_[vertex] == vertex)
        {
            return vertex;
        }
        return parent_[vertex] = find(parent_[vertex]);
    }

    void unite(size_t first, size_t second)
    {
        first = find(first);
        second = find(second);

        if (first == second)
        {
            return;
        }

        if (size_[first] < size_[second])
        {
            std::swap(first, second);
        }

        parent_[second] = first;
        size_[first] += size_[second];
    }

    bool connected(size_t first, size_t second)
    {
        return find(first) == find(second);
    }
};

class Percolation {
private:
    size_t dimension_;
    size_t opened_amount_;
    size_t virtual_top_;
    size_t virtual_bottom_;

    std::vector<char> opened_; 
    DisjointSetUnion dsu_;

    size_t get_index(size_t row, size_t column) const
    {
        return row * dimension_ + column;
    }

    bool is_correct_cell(size_t row, size_t column) const
    {
        return row < dimension_ && column < dimension_;
    }

    void connect_if_open(size_t row, size_t column, size_t current_index)
    {
        if (is_correct_cell(row, column))
        {
            size_t neighbor_index = get_index(row, column);
            if (opened_[neighbor_index] == 1)
            {
                dsu_.unite(current_index, neighbor_index);
            }
        }
    }

public:
    explicit Percolation(size_t dimension)
        : dimension_(dimension),
          opened_amount_(0),
          virtual_top_(dimension * dimension),
          virtual_bottom_(dimension * dimension + 1),
          opened_(dimension * dimension, 0),
          dsu_(dimension * dimension + 2)
    {
        if (dimension == 0)
        {
            throw std::invalid_argument("grid dimension must be positive");
        }
    }

    void open(size_t row, size_t column)
    {
        if (!is_correct_cell(row, column))
        {
            throw std::out_of_range("cell coordinates are out of range");
        }

        const size_t current_index = get_index(row, column);

        if (opened_[current_index])
        {
            return;
        }

        opened_[current_index] = 1;
        ++opened_amount_;

        if (row == 0)
        {
            dsu_.unite(current_index, virtual_top_);
        }

        if (row + 1 == dimension_)
        {
            dsu_.unite(current_index, virtual_bottom_);
        }

        if (row > 0) connect_if_open(row - 1, column, current_index);
        if (row + 1 < dimension_) connect_if_open(row + 1, column, current_index);
        if (column > 0) connect_if_open(row, column - 1, current_index);
        if (column + 1 < dimension_) connect_if_open(row, column + 1, current_index);
    }

    bool is_open(size_t row, size_t column) const
    {
        if (!is_correct_cell(row, column))
        {
            throw std::out_of_range("cell cordinates are out of range");
        }

        return opened_[get_index(row, column)] == 1;
    }

    bool percolates()
    {
        return dsu_.connected(virtual_top_, virtual_bottom_);
    }

    size_t get_opened_amount() const
    {
        return opened_amount_;
    }
};

struct PercolationStats {
private:
    size_t dimension_;
    size_t trials_;

    double mean_;
    double standard_deviation_;
    double confidence_low_;
    double confidence_high_;

    std::vector<double> thresholds_;
    std::vector<size_t> blocked_cells_cache_; 
    std::mt19937 generator_;

    double execute_one_experiment()
    {
        Percolation percolation(dimension_);

        blocked_cells_cache_.resize(dimension_ * dimension_);
        std::iota(blocked_cells_cache_.begin(), blocked_cells_cache_.end(), 0);

        std::uniform_int_distribution<size_t> distribution;
        using param_t = std::uniform_int_distribution<size_t>::param_type;

        while (!percolation.percolates())
        {
            const size_t random_position = distribution(generator_, param_t{0, blocked_cells_cache_.size() - 1});
            const size_t cell = blocked_cells_cache_[random_position];

            const size_t row = cell / dimension_;
            const size_t column = cell % dimension_;

            percolation.open(row, column);

            blocked_cells_cache_[random_position] = blocked_cells_cache_.back();
            blocked_cells_cache_.pop_back();
        }

        return static_cast<double>(percolation.get_opened_amount()) /
               static_cast<double>(dimension_ * dimension_);
    }

public:
    PercolationStats(size_t dimension, size_t trials)
        : dimension_(dimension),
          trials_(trials),
          mean_(0.0),
          standard_deviation_(0.0),
          confidence_low_(0.0),
          confidence_high_(0.0),
          generator_(std::random_device{}())
    {
        if (dimension == 0) throw std::invalid_argument("grid dimension must be positive!");
        if (trials == 0) throw std::invalid_argument("trials amount must be positive!!");

        thresholds_.reserve(trials_);
        blocked_cells_cache_.reserve(dimension * dimension); 
    }

    double get_mean() const { return mean_; }
    double get_standard_deviation() const { return standard_deviation_; }
    double get_confidence_low() const { return confidence_low_; }
    double get_confidence_high() const { return confidence_high_; }

    void execute()
    {
        thresholds_.clear();

        for (size_t trial = 0; trial < trials_; ++trial)
        {
            thresholds_.push_back(execute_one_experiment());
        }

        double sum = std::accumulate(thresholds_.begin(), thresholds_.end(), 0.0);
        mean_ = sum / static_cast<double>(trials_);

        if (trials_ <= 1)
        {
            standard_deviation_ = 0.0;
        }
        else
        {
            double squared_difference_sum = 0.0;
            for (double value : thresholds_)
            {
                const double difference = value - mean_;
                squared_difference_sum += difference * difference;
            }
            standard_deviation_ = std::sqrt(squared_difference_sum / static_cast<double>(trials_ - 1));
        }

        const double confidence_delta = 1.96 * standard_deviation_ / std::sqrt(static_cast<double>(trials_));
        confidence_low_ = mean_ - confidence_delta;
        confidence_high_ = mean_ + confidence_delta;
    }
};

int main() {
    try
    {
        size_t dimension = 0;
        size_t trials = 0;

        std::cout << "enter grid dimension: ";
        std::cin >> dimension;

        std::cout << "enter amount of experiments: ";
        std::cin >> trials;

        PercolationStats stats(dimension, trials);
        stats.execute();

        std::cout << "mean = " << stats.get_mean() << '\n';
        std::cout << "standard deviation = " << stats.get_standard_deviation() << '\n';
        std::cout << "confidence low = " << stats.get_confidence_low() << '\n';
        std::cout << "confidence high = " << stats.get_confidence_high() << '\n';
    }
    catch (const std::exception& error)
    {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
