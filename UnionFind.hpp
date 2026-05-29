#pragma once
#include <cstdio>
#include <vector>
#include <iostream>

class UnionFind { // должна обеспечивать проверку на протекание системы
private:
  std::vector<size_t> parent; // индексы - id элемента, значения - id родителя
  std::vector<size_t> size;   // размер корневого дерева
  std::size_t numbers_of_elements;
  size_t find(size_t p);               // ищет корень для вершины id = p

public:
  UnionFind(size_t n);
  size_t getParentId(size_t p) const;
  size_t getSize(size_t p) const;
  size_t getNumbersOfElements() const;

  void union_sets(size_t p, size_t q); // объединяет точки в класстеры при открытии новой вершины
  bool connect(size_t p, size_t q);    // проверяет, лежат ли 2 точки в одном класстере
};