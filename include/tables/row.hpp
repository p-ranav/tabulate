#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <tables/cell.hpp>
#include <optional>

namespace tables {

class Row {
public:
  void add_cell(const Cell& cell) {
    cells_.push_back(cell);
  }

  void print() {
    for (auto& cell : cells_) {
      cell.print();
      std::cout << " ";
    }
  }

  size_t size() {
    return cells_.size();
  }

  std::optional<Cell> get_cell(size_t index) {
    if (index < cells_.size())
      return cells_[index];
  }
  
private:
  std::vector<Cell> cells_;
};

}
