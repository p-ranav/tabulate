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
      auto cell_data = cell.data();
      auto cell_width = cell.format().width;
      // If cell contents are wider than the
      // cell format.width, then the height of
      // the row haas to be incremented to
      // fit the cell contents
      //
      // e.g., if cell text = "ABCD" and format.width = 3
      // then, cell should show:
      // ABC
      // -D
      // So, the returned height = 2
      if (cell_data.size() > cell_width) {
	result = std::max(result, cell_data.size() % cell_width);
      }
    }
    return result;
  }
  
private:
  std::vector<Cell> cells;
  Format format;
};

}
