#include "Percolation.h"
#include <queue>

Percolation::Percolation(size_t dimension)
{
    n = dimension;
    opened = 0;
    // make the field full of blocked cells (false means blocked)
    field.resize(n);
    for (size_t i = 0; i < n; i++) {
        field[i].resize(n);
        for (size_t j = 0; j < n; j++) {
            field[i][j] = false;
        }
    }
}

void Percolation::open(size_t row, size_t column)
{
    if (!is_open(row, column)) {
        field[row][column] = true;
        opened = opened + 1; // increase the counter
    }
}

bool Percolation::is_open(size_t row, size_t column) const
{
    return field[row][column];
}

void Percolation::splash_water_from_top(std::vector<std::vector<bool>> & wet) const
{
    // we put the water to every open cell in the first row and then do bfs
    std::queue<std::pair<size_t, size_t> > q;
    for (size_t j = 0; j < n; j++) {
        if (field[0][j]) {
            wet[0][j] = true;
            q.push(std::make_pair((size_t) 0, j));
        }
    }

    while (!q.empty()) {
        std::pair<size_t, size_t> cur = q.front();
        q.pop();
        size_t r = cur.first;
        size_t c = cur.second;

        // try to go to 4 sides. maybe we visit some cell two times but its ok
        if (r > 0 && field[r - 1][c] && !wet[r - 1][c]) {
            wet[r - 1][c] = true;
            q.push(std::make_pair(r - 1, c));
        }
        if (r + 1 < n && field[r + 1][c] && !wet[r + 1][c]) {
            wet[r + 1][c] = true;
            q.push(std::make_pair(r + 1, c));
        }
        if (c > 0 && field[r][c - 1] && !wet[r][c - 1]) {
            wet[r][c - 1] = true;
            q.push(std::make_pair(r, c - 1));
        }
        if (c + 1 < n && field[r][c + 1] && !wet[r][c + 1]) {
            wet[r][c + 1] = true;
            q.push(std::make_pair(r, c + 1));
        }
    }
}

bool Percolation::is_full(size_t row, size_t column) const
{
    if (!is_open(row, column)) {
        return false;
    }
    // again we splash the water from the top and just look is this cell wet
    std::vector<std::vector<bool> > wet(n, std::vector<bool>(n, false));
    splash_water_from_top(wet);
    return wet[row][column];
}

size_t Percolation::number_of_opened() const
{
    return opened;
}

bool Percolation::is_percolating() const
{
    // if the field is empty there is nothing to percolate (just in case)
    if (n == 0) {
        return false;
    }

    std::vector<std::vector<bool> > wet(n, std::vector<bool>(n, false));
    splash_water_from_top(wet);

    // look at the last row, if some cell is wet then the water came down
    bool result = false;
    for (size_t j = 0; j < n; j++) {
        if (wet[n - 1][j]) {
            result = true;
        }
    }
    return result;
}
