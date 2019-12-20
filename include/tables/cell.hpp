#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <tables/format.hpp>

namespace tables {

class Cell {
public:
  explicit Cell(const std::string& data) : data(data) {}

  std::string data() const {
    return data;
  }

  size_t size() const {
    return data.size();
  }

  Format format() const {
    return format;
  }

private:
  std::string data;
  Format format;
};

}
