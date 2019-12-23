#pragma once
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <tabulate/cell.hpp>
#include <vector>

namespace tabulate {

class Row {
public:
  explicit Row(std::shared_ptr<class TableInternal> parent) { parent_ = parent; }

  void add_cell(std::shared_ptr<Cell> cell) { cells_.push_back(cell); }

  Cell &operator[](size_t index) { return *(cells_[index]); }

  std::vector<std::shared_ptr<Cell>> cells() const { return cells_; }

  size_t size() const { return cells_.size(); }

  Format &format();
  const Format &format() const;

private:
  std::vector<std::shared_ptr<Cell>> cells_;
  std::weak_ptr<class TableInternal> parent_;
  std::optional<Format> format_;
};

} // namespace tabulate
