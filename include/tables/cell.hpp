#pragma once
#include <iostream>
#include <vector>
#include <string>

namespace tables {

class Cell {
public:
  explicit Cell(const std::string& contents) : contents_(contents) {}

  void print() {
    std::cout << contents_;
  }

  size_t get_width() {
    return contents_.size();
  }
  
private:
  std::string contents_;
};

}
