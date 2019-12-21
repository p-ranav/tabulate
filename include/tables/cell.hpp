#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <optional>
#include <tables/format.hpp>

namespace tables {

class Cell {
public:
  explicit Cell(const std::string& data) : data_(data) {}

  std::string data() const {
    return data_;
  }

  size_t size() const {
    return data_.size();
  }

  Format& format() {
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

}
