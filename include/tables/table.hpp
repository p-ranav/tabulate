#include <iostream>
#include <vector>
#include <string>
#include <tables/cell.hpp>

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

  Cell get_cell(size_t index) {
    return cells_[index];
  }
  
private:
  std::vector<Cell> cells_;
};

class Table {
public:

  void add_row(const std::vector<std::string>& cells) {
    Row row;
    for (auto& cell : cells) {
      row.add_cell(Cell(cell));
    }
    rows_.push_back(row);
  }

  void print() {
    for (size_t i = 0; i < rows_.size(); ++i) {
      rows_[i].print();
    }
  }

private:

  size_t get_column_width(size_t index) {
    size_t result{0};
    for (auto& row : rows_) {
      if (index < row.size()) {
	auto cell_width = row.get_cell(index).get_width();
	if (result < cell_width)
	  result = cell_width;
      }
    }
    return result;
  }
  
  std::vector<Row> rows_;
};

}
