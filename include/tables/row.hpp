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

  std::vector<Cell> cells() const {
    return cells_;
  }

  size_t size() const {
    return cells_.size();
  }

  std::optional<Cell> get_cell(size_t index) {
    if (index < cells_.size())
      return cells_[index];
  }
  
private:
  std::vector<Cell> cells_;
  Format format_;
};

}
