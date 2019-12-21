#pragma once
#include <iostream>
#include <optional>
#include <string>
#include <tbl/format.hpp>
#include <vector>

namespace tbl {

class Cell {
public:
  explicit Cell(const std::string &data) : data_(data) {}

  std::string data() const { return data_; }

  size_t size() const { return data_.size(); }

  Format &format() {
    if (!format_.has_value())
      format_ = inherited_format_;
    return format_.value();
  }

private:
  friend class Row;
  friend class Table;
  std::string data_;
  Format inherited_format_;
  std::optional<Format> format_;
};

} // namespace tbl
