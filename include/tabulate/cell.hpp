#pragma once
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <tabulate/format.hpp>
#include <vector>

namespace tabulate {

class Cell {
public:
  explicit Cell(std::shared_ptr<class Row> parent) : parent_(parent) {}

  void set_text(const std::string &text) { data_ = text; }

  const std::string &get_text() { return data_; }

  size_t size() const { return data_.size(); }

  Format &format();

private:
  std::string data_;
  std::weak_ptr<class Row> parent_;
  std::optional<Format> format_;
};

} // namespace tabulate
