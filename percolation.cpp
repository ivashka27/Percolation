#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <cmath>
#include <iomanip>

using namespace std;


// DSU (Union find), чтобы быстро соединять клетки
class DSU {
private:
    vector<int> parent;
    vector<int> sz;

public:
    DSU(int n) {
        parent.resize(n);
        sz.assign(n, 1);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int v) {
        if (parent[v] == v) return v;
        parent[v] = find(parent[v]);
        return parent[v];
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a == b) return;

        if (sz[a] < sz[b]) swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
    }

    bool connected(int a, int b) {
        return find(a) == find(b);
    }
};


class Percolation {
private:
    int n;
    int opened;
    vector<bool> open_cell;

    // uf нужен для проверки протекания с верха до низа
    // uf_top нужен для is_full(), чтобы не было ошибки backwash
    DSU uf;
    DSU uf_top;

    int virtual_top;
    int virtual_bottom;

    int id(int row, int col) const {
        return row * n + col;
    }

    bool inside(int row, int col) const {
        return row >= 0 && row < n && col >= 0 && col < n;
    }

public:
    Percolation(int dimension):
          n(dimension),
          opened(0),
          open_cell(dimension * dimension, false),
          uf(dimension * dimension + 2),
          uf_top(dimension * dimension + 1),
          virtual_top(dimension * dimension),
          virtual_bottom(dimension * dimension + 1) {
    }

    void open(int row, int col) {
        if (!inside(row, col)) return;
        if (is_open(row, col)) return;

        int current = id(row, col);
        open_cell[current] = true;
        opened++;

        if (row == 0) {
            uf.unite(current, virtual_top);
            uf_top.unite(current, virtual_top);
        }

        if (row == n - 1) {
            uf.unite(current, virtual_bottom);
        }

        int dr[4] = {-1, 1, 0, 0};
        int dc[4] = {0, 0, -1, 1};

        for (int i = 0; i < 4; i++) {
            int nr = row + dr[i];
            int nc = col + dc[i];

            if (inside(nr, nc) && is_open(nr, nc)) {
                int neighbour = id(nr, nc);
                uf.unite(current, neighbour);
                uf_top.unite(current, neighbour);
            }
        }

    }


    bool is_open(int row, int col) const {
        if (!inside(row, col)) return false;
        return open_cell[id(row, col)];
    }

    bool is_full(int row, int col) {
        if (!is_open(row, col)) return false;
        return uf_top.connected(id(row, col), virtual_top);
    }

    int number_of_open_sites() const {
        return opened;
    }

    bool has_percolation() {
        return uf.connected(virtual_top, virtual_bottom);
    }
};


struct PercolationStats {
private:
    int dimension;
    int trials;
    vector<double> results;

    double mean_value = 0.0;
    double std_value = 0.0;
    double conf_low = 0.0;
    double conf_high = 0.0;

    mt19937 gen;

    double one_experiment() {
        Percolation p(dimension);

        vector<int> cells(dimension * dimension);
        iota(cells.begin(), cells.end(), 0);
        shuffle(cells.begin(), cells.end(), gen);

        for (int cell : cells) {
            int row = cell / dimension;
            int col = cell % dimension;

            p.open(row, col);

            if (p.has_percolation()) {
                return (double)p.number_of_open_sites() / (dimension * dimension);
            }
        }

        return 1.0;
    }

public:
    PercolationStats(size_t dimension, size_t trials)
        : dimension((int)dimension),
          trials((int)trials),
          gen(random_device{}()) {
    }

    double get_mean() const {
        return mean_value;
    }

    double get_standard_deviation() const {
        return std_value;
    }

    double get_confidence_low() const {
        return conf_low;
    }

    double get_confidence_high() const {
        return conf_high;
    }

    void execute() {
        results.clear();

        for (int i = 0; i < trials; i++) {
            results.push_back(one_experiment());
        }

        double sum = 0.0;
        for (double x : results) {
            sum += x;
        }
        mean_value = sum / trials;

        double sq_sum = 0.0;
        for (double x : results) {
            sq_sum += (x - mean_value) * (x - mean_value);
        }

        if (trials > 1) {
            std_value = sqrt(sq_sum / (trials - 1));
        } else {
            std_value = 0.0;
        }

        double delta = 1.96 * std_value / sqrt(trials);
        conf_low = mean_value - delta;
        conf_high = mean_value + delta;
    }
};


int main(int argc, char* argv[]) {
    if (argc != 3) {
        cout << "Usage: " << argv[0] << " n trials\n";
        return 0;
    }

    int n = stoi(argv[1]);
    int trials = stoi(argv[2]);

    if (n <= 0 || trials <= 0) {
        cout << "n и trials должны быть положительными\n";
        return 0;
    }


    PercolationStats stats(n, trials);
    stats.execute();

    cout << fixed << setprecision(6);
    cout << "mean                       = " << stats.get_mean() << '\n';
    cout << "Стандартное отклонение     = " << stats.get_standard_deviation() << '\n';
    cout << "95% доверительный интервал = ["
         << stats.get_confidence_low() << ", "
         << stats.get_confidence_high() << "]\n";

    return 0;
}
