#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <utility>
#include <vector>

class Percolation {
private:
  size_t dimension;
  size_t nOpenCells = 0;
  std::vector<size_t> parents;
  std::vector<size_t> sizes;
  std::vector<char> isOpen;
  std::vector<size_t> order;
  size_t TOP_INDEX = 0;
  size_t BOTTOM_INDEX = 0;

  size_t find_parent(size_t index) {
    if (this->parents[index] == index) {
      return index;
    }
    this->parents[index] = find_parent(this->parents[index]);
    return this->parents[index];
  }

  void union_sets(size_t p, size_t q) {
    size_t rootP = find_parent(p);
    size_t rootQ = find_parent(q);

    if (rootP == rootQ) {
      return;
    }

    if (this->sizes[rootP] > this->sizes[rootQ]) {
      this->parents[rootQ] = rootP;
      this->sizes[rootP] += this->sizes[rootQ];
    } else {
      this->parents[rootP] = rootQ;
      this->sizes[rootQ] += this->sizes[rootP];
    }
  }

  bool in_bounds(size_t row, size_t col) const {
    return row < dimension && col < dimension;
  }

  void update() {
    const size_t indexToUpdate = order[nOpenCells];

    this->isOpen[indexToUpdate] = 1;
    nOpenCells++;

    const std::pair<size_t, size_t> rc = toRowCol(indexToUpdate);
    const size_t row = rc.first;
    const size_t col = rc.second;

    // Connect to virtual nodes based on position (row), not component root.
    if (row == 0) {
      union_sets(TOP_INDEX, indexToUpdate);
    }
    if (row + 1 == dimension) {
      union_sets(BOTTOM_INDEX, indexToUpdate);
    }

    const std::vector<std::pair<size_t, size_t>> neighbours = {
        {row + 1, col},
        {row == 0 ? dimension : row - 1, col},
        {row, col + 1},
        {row, col == 0 ? dimension : col - 1}};

    for (const auto &nei : neighbours) {
      const size_t neiRow = nei.first;
      const size_t neiCol = nei.second;
      if (!in_bounds(neiRow, neiCol)) {
        continue;
      }
      const size_t indexPair = toIndex(neiRow, neiCol);
      if (this->isOpen[indexPair]) {
        union_sets(indexToUpdate, indexPair);
      }
    }
  }

  size_t toIndex(size_t row, size_t col) const { return row * dimension + col; }
  std::pair<size_t, size_t> toRowCol(size_t index) const {
    return {index / dimension, index % dimension};
  }
  bool percolates() {
    return find_parent(TOP_INDEX) == find_parent(BOTTOM_INDEX);
  }

public:
  Percolation(size_t dimension, std::mt19937 &rng) {
    this->dimension = dimension;

    const size_t gridSize = dimension * dimension;
    this->TOP_INDEX = gridSize;
    this->BOTTOM_INDEX = gridSize + 1;

    this->parents.resize(gridSize + 2);
    this->sizes.resize(gridSize + 2);
    this->isOpen.assign(gridSize, 0);
    this->order.resize(gridSize);

    for (size_t i = 0; i < gridSize + 2; i++) {
      this->parents[i] = i;
      this->sizes[i] = 1;
    }

    for (size_t i = 0; i < gridSize; i++) {
      this->order[i] = i;
    }

    std::shuffle(this->order.begin(), this->order.end(), rng);
  }

  size_t execute() {
    while (!percolates()) {
      update();
    }
    return nOpenCells;
  }
};

struct PercolationStats {
  /**
   * Construct a new Percolation Stats object
   * @param dimension dimension of percolation grid
   * @param trials amount of experiments
   */
  PercolationStats(size_t dimension, size_t trials) {
    this->dimension = dimension;
    this->trials = trials;
    this->thresholds.resize(trials);
  }

  /**
   * Returns mean of percolation threshold (x¯ from description)
   */
  double get_mean() const {
    double sum = 0.0;
    for (size_t i = 0; i < this->trials; i++) {
      sum += this->thresholds[i];
    }
    return sum / static_cast<double>(this->trials);
  }

  /**
   * Returns standard deviation of percolation threshold (s from description)
   */
  double get_standard_deviation() const {
    double mean = get_mean();
    double sum = 0;
    for (size_t i = 0; i < this->trials; i++) {
      sum += (this->thresholds[i] - mean) * (this->thresholds[i] - mean);
    }
    return sqrt(sum / static_cast<double>(this->trials - 1));
  }

  /**
   * Returns log edge of condidence interval
   */
  double get_confidence_low() const {
    double mean = get_mean();
    double std = get_standard_deviation();
    return mean - 1.96 * std / sqrt(static_cast<double>(this->trials));
  }

  /**
   * Returns high edge of confidence interval
   */
  double get_confidence_high() const {
    double mean = get_mean();
    double std = get_standard_deviation();
    return mean + 1.96 * std / sqrt(static_cast<double>(this->trials));
  }

  /**
   * Makes all experiments, calculates statistic values
   */
  void execute() {
    std::mt19937 rng(this->seed);
    const double totalCells = static_cast<double>(dimension * dimension);

    for (size_t i = 0; i < this->trials; i++) {
      Percolation p(this->dimension, rng);
      const size_t openCells = p.execute();
      this->thresholds[i] = static_cast<double>(openCells) / totalCells;
    }
  }

private:
  uint32_t seed = 42;
  size_t dimension;
  size_t trials;
  std::vector<double> thresholds;
};

int main() {
  std::vector<std::pair<size_t, int>> configurations = {
      {10, 10}, {100, 100}, {200, 100}, {1000, 100}, {100, 1000}};
  for (const auto &cfg : configurations) {
    const size_t n = cfg.first;
    const int t = cfg.second;
    std::cout << "Executing " << n << "x" << n << " with " << t << " trials"
              << std::endl;
    PercolationStats p(n, t);
    p.execute();
    std::cout << "Mean: " << p.get_mean() << " "
              << "Std: " << p.get_standard_deviation() << " "
              << "Low: " << p.get_confidence_low() << " "
              << "High: " << p.get_confidence_high() << std::endl
              << "--------------------------------" << std::endl;
  }
  return 0;
}