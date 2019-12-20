#include <iostream>
#include <vector>
#include <string>
#include <tables/row.hpp>

namespace tables {

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
	size_t cell_width{0};
	auto cell = row.get_cell(index);
	if (cell.has_value())
	  cell_width = cell.value().get_width();
	if (result < cell_width)
	  result = cell_width;
      }
    }
    return result;
  }

  size_t get_row_height(size_t index) {
    size_t result{0};
    if (index < rows_.size()) {
      
    }
    return result;
  }
  
  std::vector<Row> rows_;
};

}
