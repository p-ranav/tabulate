#pragma once
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <tabulate/format.hpp>
#include <vector>

namespace tabulate {

class Cell {
public:
  void set_text(const std::string &text) { data_ = text; }

  size_t size() const { return data_.size(); }

  Format &format();

private:
  explicit Cell(class Row &parent) : parent_(parent) {}

  friend class Row;
  friend class Table;
  std::string data_;
  std::reference_wrapper<class Row> parent_;
  std::optional<Format> format_;
};

} // namespace tabulate
