#include "Percolation.h"
#include <vector>
#include <queue>

Percolation::Percolation(size_t n):
	size(n),
	grid(n * n, false),
	open_sites(0) {}


void Percolation::open(size_t row, size_t col) {
	if (row < size && col < size && !grid[row * size + col]) {
		grid[row * size + col] = true;
		open_sites++;
	}
}


bool Percolation::is_open(size_t row, size_t col) const {
	if (row >= size || col >= size) return false;
		return grid[row * size + col];
}


size_t Percolation::get_open_sites_count() const { return open_sites; }


bool Percolation::percolates() const {

	std::vector<bool> visited(size * size, false);
	std::queue<size_t> q;

	for (size_t col = 0; col < size; ++col) {
		if (is_open(0, col)) {
			q.push(0 * size + col);
			visited[0 * size + col] = true;
		}
	}

	const std::vector<std::pair<int, int>> directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };


	while (!q.empty()) {
		size_t current = q.front();
		q.pop();

		size_t row = current / size;
		size_t col = current % size;

		if (row == size - 1) { return true; }

		
		for (const auto& dir : directions) {
			int new_row = static_cast<int>(row) + dir.first;
			int new_col = static_cast<int>(col) + dir.second;
			if (new_row >= 0 && new_row < static_cast<int>(size) && new_col >= 0 && new_col < static_cast<int>(size)) {
				size_t neighbor_idx = new_row * size + new_col;
				if (is_open(new_row, new_col) && !visited[neighbor_idx]) {
					visited[neighbor_idx] = true;
					q.push(neighbor_idx);
				}
			}
		}
	}

	return false;
}