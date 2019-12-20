#pragma once
#include <iostream>
#include <vector>
#include <string>
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

  Format format() const {
    return format_;
  }

private:
  std::string data_;
  Format format_;
};

}
