#include "UnionFind.hpp"


UnionFind::UnionFind(size_t n) : parent(n), size(n), numbers_of_elements(n) {
  for (std::size_t i = 0; i < n; i++) {
    parent[i] = i;
    size[i] = 1;
  }
};

size_t UnionFind::getParentId(size_t p) const { return parent[p]; }

size_t UnionFind::getSize(size_t p) const { return size[p]; }

size_t UnionFind::getNumbersOfElements() const { return numbers_of_elements; }

size_t UnionFind::find(size_t p) {
  if (parent[p] == p) {
    return p;
  }

  size_t root_id = find(parent[p]);
  parent[p] = root_id;
  return root_id;
}

void UnionFind::union_sets(size_t p, size_t q) {
  if (p >= numbers_of_elements or q >= numbers_of_elements) {
    std::cerr << "Неверный id элемента в UnionFind::union_sets.  p, q, numbers_of_elements: " << p << " " << q << " " << getNumbersOfElements() << std::endl;
    return;
  }
  std::size_t root_p = find(p);
  std::size_t root_q = find(q);

  if (root_p != root_q) {
    if (size[root_p] < size[root_q]) {
      parent[root_p] = root_q;
      size[root_q] += size[root_p];
    }

    else {
      parent[root_q] = root_p;
      size[root_p] += size[root_q];
    }
  }
}

bool UnionFind::connect(size_t p, size_t q) {
  size_t root_p = find(p);
  size_t root_q = find(q);
  return root_p == root_q;
}