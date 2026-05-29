#pragma once

#include "UnionFind.hpp"
#include <cstddef>
#include <vector>

class Percolation {
private:
  std::size_t dimension;
  std::size_t numberOfCells;
  std::size_t openedCellsCount;
  std::vector<std::vector<bool>> cellsOpened;
  UnionFind uf;

public:
  explicit Percolation(std::size_t dimension);
  std::size_t coordinateToId(std::size_t row, std::size_t col) const;
  bool isOpen(std::size_t row, std::size_t col) const;
  bool percolates();
  std::size_t getNumberOfCells() const;
  std::size_t getNumberOfCellsInRow() const;
  std::size_t getHighBoarderId() const;
  std::size_t getLowBoarderId() const;
  std::size_t open_cells_count() const;
  void open(std::size_t row, std::size_t col);
};
