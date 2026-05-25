#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <cmath>


class UF { // Чтобы находить открытые кластеры в массиве
private:
    std::vector<int> parent;
    std::vector<int> size;
public:
    UF(int n){
        parent.resize(n);
        size.resize(n, 1);
        for (int i = 0; i < n; ++i)
            parent[i] = i;
    }
    int find(int p){
        if (parent[p] != p)
            parent[p] = find(parent[p]);
        return parent[p];
    }
    void unite(int p, int q){
        int rootP = find(p);
        int rootQ = find(q);
        if (rootP == rootQ) return;
        if (size[rootP] < size[rootQ]){
            parent[rootP] = rootQ;
            size[rootQ] += size[rootP];
        }
        else {
            parent[rootQ] = rootP;
            size[rootP] += size[rootQ];
        }
    }
    bool connected(int p, int q){
        return find(p) == find(q);
    }
};
class Percolation{
private:
    int n; 
    std::vector<bool> open_el;
    UF uf;
    int count_open;
    const int TOP_VIRTUAL;
    const int BOTTOM_VIRTUAL;

    int index(int row, int col) { return row * n + col; }
public:
    Percolation(int n): n(n), open_el(n * n, false), uf(n * n + 2),
                        count_open(0), TOP_VIRTUAL(n * n), BOTTOM_VIRTUAL(n * n + 1)
                        {}
    void open(int row, int col){
        int idx = index(row, col);
        if (open_el[idx]) return;
        open_el[idx] = true;
        ++count_open;
        if (row == 0) uf.unite(idx, TOP_VIRTUAL);
        if (row == n - 1) uf.unite(idx, BOTTOM_VIRTUAL);

        if (row > 0 && open_el[index(row - 1, col)]) uf.unite(idx, index(row - 1, col));
        if (row < n - 1 && open_el[index(row + 1, col)]) uf.unite(idx, index(row + 1, col));

        if (col > 0 && open_el[index(row, col - 1)]) uf.unite(idx, index(row, col - 1));
        if (col < n - 1 && open_el[index(row, col + 1)]) uf.unite(idx, index(row, col + 1));
    }
    bool is_percolated(){
        return uf.connected(TOP_VIRTUAL, BOTTOM_VIRTUAL);
    }
    int number_of_open_cells(){
        return count_open;
    }
};

class PercolationStats
{
private:
    int trials;
    int dimension;
    double mean = 0;
    double stddev = 0;
    double confidence_low = 0;
    double confidence_high = 0;
public:
    /**
     * Construct a new Percolation Stats object
     * @param dimension dimension of percolation grid
     * @param trials amount of experiments
     */
    PercolationStats(int dimension_, int trials_) 
        : dimension(dimension_), trials(trials_) {}

    /**
     * Returns mean of percolation threshold (x¯ from description)
     */
    double get_mean() const{ return mean; }

    /**
     * Returns standard deviation of percolation threshold (s from description)
     */
    double get_standard_deviation() const { return stddev; }

    /**
     * Returns log edge of condidence interval
     */
    double get_confidence_low() const { return confidence_low; }

    /**
     * Returns high edge of confidence interval
     */
    double get_confidence_high() const { return confidence_high; }

    /**
     * Makes all experiments, calculates statistic values
     */
    void execute(){
        std::vector<double> thresholds(trials);
        std::random_device rd;
        std::mt19937 g(rd());
        for (int t = 0; t < trials; ++t){
            Percolation p(dimension);
            std::vector<int> indexes(dimension * dimension);
            for (int i = 0; i < dimension * dimension; ++i){
                indexes[i] = i;
            }
            
            std::shuffle(indexes.begin(), indexes.end(), g);
            
            for (int idx : indexes){
                int row, col;
                row = idx / dimension;
                col = idx % dimension;
                p.open(row, col);
                if (p.is_percolated()){
                    double threshold = ((double)p.number_of_open_cells() / (dimension * dimension));
                    thresholds[t] = threshold;
                    break;
                }
            }
        }
        double sum = 0.0;
        for (double x : thresholds) sum += x;
        mean = sum / trials;

        double sq_sum = 0.0;
        for (double x : thresholds) sq_sum += (x - mean) * (x - mean);
        stddev = std::sqrt(sq_sum / (trials - 1));

        const double z = 1.96;
        confidence_low = mean - z * stddev / std::sqrt(trials);
        confidence_high = mean + z * stddev / std::sqrt(trials);
    }
};
int main(){
    PercolationStats stats(200, 100);
    stats.execute();
    std::cout << "mean = " << stats.get_mean() << '\n';
    std::cout << "stddev = " << stats.get_standard_deviation() << '\n';
    std::cout << "95\% CI: [" << stats.get_confidence_low() << ','
              << stats.get_confidence_high() << "]\n";
    return 0;
}