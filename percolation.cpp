#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <cmath>
#include <algorithm>

using namespace std;

struct DSU {
    vector<int> parent;
    vector<int> sz;

    DSU(int n) {
        parent.resize(n);
        sz.resize(n, 1);
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }

    void unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            if (sz[root_i] < sz[root_j]) {
                parent[root_i] = root_j;
                sz[root_j] += sz[root_i];
            }
            else {
                parent[root_j] = root_i;
                sz[root_i] += sz[root_j];
            }
        }
    }

    bool connected(int i, int j) {
        return find(i) == find(j);
    }
};

class Percolation {
    int n;
    vector<bool> opened;
    int open_count;
    DSU dsu;
    int top, bottom;

public:
    Percolation(int n_val) : n(n_val), opened(n_val* n_val, false), open_count(0), dsu(n_val* n_val + 2) {
        top = n * n;
        bottom = n * n + 1;
    }

    int get_id(int r, int c) {
        return r * n + c;
    }

    void open(int r, int c) {
        int id = get_id(r, c);
        if (opened[id]) return;

        opened[id] = true;
        open_count++;

        if (r == 0) dsu.unite(id, top);
        if (r == n - 1) dsu.unite(id, bottom);

        int dr[] = { -1, 1, 0, 0 };
        int dc[] = { 0, 0, -1, 1 };

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];
            if (nr >= 0 && nr < n && nc >= 0 && nc < n && opened[get_id(nr, nc)]) {
                dsu.unite(id, get_id(nr, nc));
            }
        }
    }

    bool percolates() {
        return dsu.connected(top, bottom);
    }

    int get_open_count() {
        return open_count;
    }
};

struct PercolationStats
{
    /**
     * Construct a new Percolation Stats object
     * @param dimension dimension of percolation grid
     * @param trials amount of experiments
     */
    PercolationStats(size_t dimension, size_t trials);

    /**
     * Returns mean of percolation threshold (x¯ from description)
     */
    double get_mean() const;

    /**
     * Returns standard deviation of percolation threshold (s from description)
     */
    double get_standard_deviation() const;

    /**
     * Returns log edge of condidence interval
     */
    double get_confidence_low() const;

    /**
     * Returns high edge of confidence interval
     */
    double get_confidence_high() const;

    /**
     * Makes all experiments, calculates statistic values
     */
    void execute();

private:
    size_t n;
    size_t t;
    double mean_val;
    double std_dev;
};

PercolationStats::PercolationStats(size_t dimension, size_t trials) {
    n = dimension;
    t = trials;
    mean_val = 0;
    std_dev = 0;
}

double PercolationStats::get_mean() const {
    return mean_val;
}

double PercolationStats::get_standard_deviation() const {
    return std_dev;
}

double PercolationStats::get_confidence_low() const {
    return mean_val - (1.96 * std_dev) / sqrt(t);
}

double PercolationStats::get_confidence_high() const {
    return mean_val + (1.96 * std_dev) / sqrt(t);
}

void PercolationStats::execute() {
    vector<double> thresholds(t);
    random_device rd;
    mt19937 gen(rd());

    int dim = n;
    for (size_t i = 0; i < t; i++) {
        Percolation p(dim);
        vector<pair<int, int>> cells;

        for (int r = 0; r < dim; r++) {
            for (int c = 0; c < dim; c++) {
                cells.push_back({ r, c });
            }
        }

        shuffle(cells.begin(), cells.end(), gen);

        for (auto cell : cells) {
            p.open(cell.first, cell.second);
            if (p.percolates()) {
                thresholds[i] = (double)p.get_open_count() / (dim * dim);
                break;
            }
        }
    }

    double sum = 0;
    for (double x : thresholds) sum += x;
    mean_val = sum / t;

    double sum_sq = 0;
    for (double x : thresholds) {
        sum_sq += (x - mean_val) * (x - mean_val);
    }

    if (t > 1) {
        std_dev = sqrt(sum_sq / (t - 1));
    }
    else {
        std_dev = 0;
    }
}
