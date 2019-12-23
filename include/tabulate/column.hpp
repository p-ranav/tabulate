#pragma once
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <tabulate/cell.hpp>
#include <tabulate/column_format.hpp>
#include <vector>
#include <algorithm>

namespace tabulate {

class Column {
public:
  explicit Column(std::shared_ptr<class TableInternal> parent) { parent_ = parent; }

  void add_cell(Cell &cell) { cells_.push_back(cell); }

  Cell &operator[](size_t index) { return cells_[index]; }

  std::vector<std::reference_wrapper<Cell>> cells() const { return cells_; }

  size_t size() const { return cells_.size(); }

  ColumnFormat format() { return ColumnFormat(*this); }

private:
  friend class ColumnFormat;
  friend class Printer;

  // Returns the column width as configured
  // For each cell in the column, check the cell.format.width
  // property and return the largest configured column width
  // This is used to ensure that all cells in a column are
  // aligned when printing the column
  size_t get_configured_width() {
    size_t result{0};
    for (size_t i = 0; i < size(); ++i) {
      auto cell = cells_[i];
      auto format = cell.get().format();
      if (format.width_.has_value())
        result = std::max(result, format.width_.value());
    }
    return result;    
  }

  // Computes the width of the column based on cell contents
  // and configured cell padding
  // For each cell, compute padding_left + cell_contents + padding_right
  // and return the largest value
  // 
  // This is useful when no cell.format.width is configured
  // Call get_configured_width()
  // - If this returns 0, then use get_computed_width()
  size_t get_computed_width() {
    size_t result{0};
    for (size_t i = 0; i < size(); ++i) {
      result = std::max(result, get_cell_width(i));
    }
    return result;
  }

  // Returns padding_left + cell_contents.size() + padding_right
  // for a given cell in the column
  size_t get_cell_width(size_t cell_index) {
    size_t result{0};
    Cell& cell = cells_[cell_index].get();
    auto format = cell.format();
    if (format.padding_left_.has_value())
      result += format.padding_left_.value();
    result += cell.get_text().size();
    if (format.padding_right_.has_value())
      result += format.padding_right_.value();

    return result;
  }

  std::vector<std::reference_wrapper<Cell>> cells_;
  std::weak_ptr<class TableInternal> parent_;
};

Format &ColumnFormat::width(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().width(value);
  return *this;
}

Format &ColumnFormat::height(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().height(value);
  return *this;
}

} // namespace tabulate
