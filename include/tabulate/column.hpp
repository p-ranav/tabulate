#pragma once
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <tabulate/cell.hpp>
#include <tabulate/column_format.hpp>
#include <vector>

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
