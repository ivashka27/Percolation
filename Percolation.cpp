#include "Percolatioin.hpp"

#include <array>
#include <iostream>

Percolation::Percolation(std::size_t dimension)
    : dimension(dimension == 0 ? 1 : dimension),
      numberOfCells((dimension == 0 ? 1 : dimension) * (dimension == 0 ? 1 : dimension)),
      openedCellsCount(0),
      cellsOpened(dimension == 0 ? 1 : dimension, std::vector<bool>(dimension == 0 ? 1 : dimension, false)),
      uf((dimension == 0 ? 1 : dimension) * (dimension == 0 ? 1 : dimension) + 2) {
  if (dimension == 0) {
    std::cerr << "dimension 0" << std::endl;
  }
}

std::size_t Percolation::coordinateToId(std::size_t row, std::size_t col) const {
  return row * dimension + col;
}

void Percolation::open(std::size_t row, std::size_t col) {
  if (row >= dimension || col >= dimension) {
    std::cerr << "Неверные координаты для метода Percolation::open. Значения row, col: " << row << "," << col
              << std::endl;
    return;
  }

  if (cellsOpened[row][col]) {
    return;
  }

  cellsOpened[row][col] = true;
  ++openedCellsCount;

  const std::size_t curr = coordinateToId(row, col);
  static const std::array<std::array<int, 2>, 4> directions = {{{-1, 0}, {1, 0}, {0, -1}, {0, 1}}};

  for (const auto& direction : directions) {
    const int nr = static_cast<int>(row) + direction[0];
    const int nc = static_cast<int>(col) + direction[1];

    if (nr >= 0 && nr < static_cast<int>(dimension) && nc >= 0 && nc < static_cast<int>(dimension)) {
      const std::size_t neighborRow = static_cast<std::size_t>(nr);
      const std::size_t neighborCol = static_cast<std::size_t>(nc);

      if (isOpen(neighborRow, neighborCol)) {
        uf.union_sets(curr, coordinateToId(neighborRow, neighborCol));
      }
    }
  }

  if (row == 0) {
    uf.union_sets(curr, getHighBoarderId());
  }

  if (row == dimension - 1) {
    uf.union_sets(curr, getLowBoarderId());
  }
}

std::size_t Percolation::getNumberOfCells() const { return numberOfCells; }

std::size_t Percolation::getNumberOfCellsInRow() const { return dimension; }

std::size_t Percolation::getHighBoarderId() const { return numberOfCells; }

std::size_t Percolation::getLowBoarderId() const { return numberOfCells + 1; }

bool Percolation::isOpen(std::size_t row, std::size_t col) const {
  if (row >= dimension || col >= dimension) {
    std::cerr << "Неправильные координаты в Percolation::isOpen. row, col: " << row << " " << col << std::endl;
    return false;
  }
  return cellsOpened[row][col];
}

std::size_t Percolation::open_cells_count() const { return openedCellsCount; }

bool Percolation::percolates() {
  return uf.connect(getHighBoarderId(), getLowBoarderId());
}
