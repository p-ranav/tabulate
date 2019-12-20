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
    cells.push_back(cell);
  }

  std::vector<Cell> cells() const {
    return cells;
  }

  size_t size() const {
    return cells.size();
  }

  std::optional<Cell> get_cell(size_t index) {
    if (index < cells.size())
      return cells[index];
  }

  size_t height() const {
    size_t result{1};
    for (auto& cell: cells) {
      // Use format.width
      // if cell text = "ABCD" and format.width = 3
      // then, cell should show:
      // ABC
      // -D
      // So, the returned height = 2
      auto cell_data = cell.data();
      auto cell_width = cell.format().width;
    }
    return result;
  }
  
private:
  std::vector<Cell> cells;
  Format format;
};

}
