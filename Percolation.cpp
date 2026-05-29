#include <iostream>
#include <vector>
#include <random>
#include <numeric>
#include <cmath>
#include <stdexcept>

class DSU
{
private:
    std::vector<size_t> parent;
    std::vector<size_t> sz;

public:
    explicit DSU(size_t n)
        : parent(n), sz(n, 1)
    {
        for (size_t i = 0; i < n; ++i)
        {
            parent[i] = i;
        }
    }

    size_t find(size_t v)
    {
        if (parent[v] == v)
        {
            return v;
        }

        parent[v] = find(parent[v]);
        return parent[v];
    }

    void unite(size_t a, size_t b)
    {
        a = find(a);
        b = find(b);

        if (a == b)
        {
            return;
        }

        if (sz[a] < sz[b])
        {
            std::swap(a, b);
        }

        parent[b] = a;
        sz[a] += sz[b];
    }

    bool connected(size_t a, size_t b)
    {
        return find(a) == find(b);
    }
};

class Percolation
{
private:
    size_t n;
    std::vector<bool> open_sites;
    DSU dsu;

    size_t top_virtual;
    size_t bottom_virtual;
    size_t open_count;

    size_t index(size_t row, size_t col) const
    {
        return row * n + col;
    }

    bool valid(size_t row, size_t col) const
    {
        return row < n && col < n;
    }

public:
    explicit Percolation(size_t dimension)
        : n(dimension),
        open_sites(n* n, false),
        dsu(n* n + 2),
        top_virtual(n* n),
        bottom_virtual(n* n + 1),
        open_count(0)
    {
        if (dimension == 0)
        {
            throw std::invalid_argument("Dimension must be > 0");
        }
    }

    void open(size_t row, size_t col)
    {
        if (!valid(row, col))
        {
            throw std::out_of_range("Invalid coordinates");
        }

        size_t current = index(row, col);

        if (open_sites[current])
        {
            return;
        }

        open_sites[current] = true;
        ++open_count;

        if (row == 0)
        {
            dsu.unite(current, top_virtual);
        }

        if (row == n - 1)
        {
            dsu.unite(current, bottom_virtual);
        }

        const int dr[] = { -1, 1, 0, 0 };
        const int dc[] = { 0, 0, -1, 1 };

        for (int k = 0; k < 4; ++k)
        {
            int nr = static_cast<int>(row) + dr[k];
            int nc = static_cast<int>(col) + dc[k];

            if (nr >= 0 && nr < static_cast<int>(n) &&
                nc >= 0 && nc < static_cast<int>(n))
            {
                size_t neighbor = index(nr, nc);

                if (open_sites[neighbor])
                {
                    dsu.unite(current, neighbor);
                }
            }
        }
    }

    bool is_open(size_t row, size_t col) const
    {
        if (!valid(row, col))
        {
            throw std::out_of_range("Invalid coordinates");
        }

        return open_sites[index(row, col)];
    }

    bool percolates()
    {
        return dsu.connected(top_virtual, bottom_virtual);
    }

    size_t get_open_sites_count() const
    {
        return open_count;
    }
};

struct PercolationStats
{
private:
    size_t dimension;
    size_t trials;

    double mean = 0.0;
    double stddev = 0.0;
    double conf_low = 0.0;
    double conf_high = 0.0;

    std::vector<double> thresholds;

public:

    PercolationStats(size_t dimension, size_t trials)
        : dimension(dimension), trials(trials)
    {
        if (dimension == 0 || trials == 0)
        {
            throw std::invalid_argument("Arguments must be > 0");
        }
    }


    double get_mean() const
    {
        return mean;
    }


    double get_standard_deviation() const
    {
        return stddev;
    }


    double get_confidence_low() const
    {
        return conf_low;
    }


    double get_confidence_high() const
    {
        return conf_high;
    }

 
    void execute()
    {
        thresholds.clear();

        std::random_device rd;
        std::mt19937 gen(rd());

        for (size_t t = 0; t < trials; ++t)
        {
            Percolation perc(dimension);

            std::vector<size_t> cells(dimension * dimension);
            std::iota(cells.begin(), cells.end(), 0);

            std::shuffle(cells.begin(), cells.end(), gen);

            size_t ptr = 0;

            while (!perc.percolates())
            {
                size_t id = cells[ptr++];

                size_t row = id / dimension;
                size_t col = id % dimension;

                perc.open(row, col);
            }

            double threshold =
                static_cast<double>(perc.get_open_sites_count()) /
                static_cast<double>(dimension * dimension);

            thresholds.push_back(threshold);
        }

        mean = std::accumulate(
            thresholds.begin(),
            thresholds.end(),
            0.0) /
            static_cast<double>(trials);

        double sum = 0.0;

        for (double x : thresholds)
        {
            sum += (x - mean) * (x - mean);
        }

        if (trials > 1)
        {
            stddev = std::sqrt(sum / static_cast<double>(trials - 1));
        }
        else
        {
            stddev = 0.0;
        }

        double delta = 1.96 * stddev / std::sqrt(static_cast<double>(trials));

        conf_low = mean - delta;
        conf_high = mean + delta;
    }
};

