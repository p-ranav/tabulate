#pragma once
#include <algorithm>
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

  class CellIterator {
  public:
    CellIterator(std::vector<std::reference_wrapper<Cell>>::iterator ptr): ptr(ptr){}

    CellIterator operator++() { ++ptr; return *this; }
    bool operator!=(const CellIterator & other) const { return ptr != other.ptr; }
    Cell& operator*() { return *ptr; }
  private:
    std::vector<std::reference_wrapper<Cell>>::iterator ptr;
  };

  auto begin() { return CellIterator(cells_.begin()); }
  auto end() { return CellIterator(cells_.end()); }

private:
  friend class ColumnFormat;
  friend class Printer;

  // Returns the column width as configured
  // For each cell in the column, check the cell.format.width
  // property and return the largest configured column width
  // This is used to ensure that all cells in a column are
  // aligned when printing the column
  size_t get_configured_width() {
    size_t result{0};
    for (size_t i = 0; i < size(); ++i) {
      auto cell = cells_[i];
      auto format = cell.get().format();
      if (format.width_.has_value())
        result = std::max(result, format.width_.value());
    }
    return result;
  }

  // Computes the width of the column based on cell contents
  // and configured cell padding
  // For each cell, compute padding_left + cell_contents + padding_right
  // and return the largest value
  //
  // This is useful when no cell.format.width is configured
  // Call get_configured_width()
  // - If this returns 0, then use get_computed_width()
  size_t get_computed_width() {
    size_t result{0};
    for (size_t i = 0; i < size(); ++i) {
      result = std::max(result, get_cell_width(i));
    }
    return result;
  }

  // Returns padding_left + cell_contents.size() + padding_right
  // for a given cell in the column
  size_t get_cell_width(size_t cell_index) {
    size_t result{0};
    Cell &cell = cells_[cell_index].get();
    auto format = cell.format();
    if (format.padding_left_.has_value())
      result += format.padding_left_.value();

    // Check if input text has newlines
    auto text = cell.get_text();
    auto split_lines = Format::split_lines(text, "\n");

    // If there are no newlines in input, set column_width = text.size()
    if (split_lines.size() == 1) {
      result += cell.size();
    } else {
      // There are newlines in input
      // Find widest substring in input and use this as column_width
      size_t widest_sub_string_size{0};
      for (auto &line : split_lines)
        if (get_sequence_length(line) > widest_sub_string_size)
          widest_sub_string_size = get_sequence_length(line);
      result += widest_sub_string_size;
    }

    if (format.padding_right_.has_value())
      result += format.padding_right_.value();

    return result;
  }

  std::vector<std::reference_wrapper<Cell>> cells_;
  std::weak_ptr<class TableInternal> parent_;
};

ColumnFormat &ColumnFormat::width(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().width(value);
  return *this;
}

ColumnFormat &ColumnFormat::height(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().height(value);
  return *this;
}

ColumnFormat &ColumnFormat::margin(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().margin(value);
  return *this;
}

ColumnFormat &ColumnFormat::margin_left(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().margin_left(value);
  return *this;
}

ColumnFormat &ColumnFormat::margin_right(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().margin_right(value);
  return *this;
}

ColumnFormat &ColumnFormat::margin_top(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().margin_top(value);
  return *this;
}

ColumnFormat &ColumnFormat::margin_bottom(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().margin_bottom(value);
  return *this;
}

ColumnFormat &ColumnFormat::padding(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().padding(value);
  return *this;
}

ColumnFormat &ColumnFormat::padding_left(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().padding_left(value);
  return *this;
}

ColumnFormat &ColumnFormat::padding_right(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().padding_right(value);
  return *this;
}

ColumnFormat &ColumnFormat::padding_top(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().padding_top(value);
  return *this;
}

ColumnFormat &ColumnFormat::padding_bottom(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().padding_bottom(value);
  return *this;
}

ColumnFormat &ColumnFormat::border(const std::string &value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border(value);
  return *this;
}

ColumnFormat &ColumnFormat::border_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_color(value);
  return *this;
}

ColumnFormat &ColumnFormat::border_background_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_background_color(value);
  return *this;
}

ColumnFormat &ColumnFormat::border_left(const std::string &value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_left(value);
  return *this;
}

ColumnFormat &ColumnFormat::border_left_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_left_color(value);
  return *this;
}

ColumnFormat &ColumnFormat::border_left_background_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_left_background_color(value);
  return *this;
}

ColumnFormat &ColumnFormat::border_right(const std::string &value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_right(value);
  return *this;
}

ColumnFormat &ColumnFormat::border_right_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_right_color(value);
  return *this;
}

ColumnFormat &ColumnFormat::border_right_background_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_right_background_color(value);
  return *this;
}

ColumnFormat &ColumnFormat::border_top(const std::string &value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_top(value);
  return *this;
}

ColumnFormat &ColumnFormat::border_top_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_top_color(value);
  return *this;
}

ColumnFormat &ColumnFormat::border_top_background_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_top_background_color(value);
  return *this;
}

ColumnFormat &ColumnFormat::border_bottom(const std::string &value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_bottom(value);
  return *this;
}

ColumnFormat &ColumnFormat::border_bottom_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_bottom_color(value);
  return *this;
}

ColumnFormat &ColumnFormat::border_bottom_background_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_bottom_background_color(value);
  return *this;
}

ColumnFormat &ColumnFormat::corner(const std::string &value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().corner(value);
  return *this;
}

ColumnFormat &ColumnFormat::corner_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().corner_color(value);
  return *this;
}

ColumnFormat &ColumnFormat::corner_background_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().corner_background_color(value);
  return *this;
}

ColumnFormat &ColumnFormat::column_separator(const std::string &value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().column_separator(value);
  return *this;
}

ColumnFormat &ColumnFormat::column_separator_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().column_separator_color(value);
  return *this;
}

ColumnFormat &ColumnFormat::column_separator_background_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().column_separator_background_color(value);
  return *this;
}

ColumnFormat &ColumnFormat::font_align(FontAlign value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().font_align(value);
  return *this;
}

ColumnFormat &ColumnFormat::font_style(const std::vector<FontStyle> &style) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().font_style(style);
  return *this;
}

ColumnFormat &ColumnFormat::font_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().font_color(value);
  return *this;
}

ColumnFormat &ColumnFormat::font_background_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().font_background_color(value);
  return *this;
}

ColumnFormat &ColumnFormat::color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().color(value);
  return *this;
}

ColumnFormat &ColumnFormat::background_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().background_color(value);
  return *this;
}

} // namespace tabulate
