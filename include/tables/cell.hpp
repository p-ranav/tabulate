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

private:
  friend class Row;
  std::string data_;
  std::optional<Format> format_;
};

}
