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

  size_t get_width() {
    size_t result{0};
    for (size_t i = 0; i < size(); ++i) {
      result = std::max(result, get_cell_width(i));
    }
    return result;
  }

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
