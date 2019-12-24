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

  Cell &operator[](size_t index) { return cell(index); }

  Cell &cell(size_t index) { return *(cells_[index]); }

  std::vector<std::shared_ptr<Cell>> cells() const { return cells_; }

  size_t size() const { return cells_.size(); }

  Format &format();

private:
  friend class Printer;

  // Returns the row height as configured
  // For each cell in the row, check the cell.format.height
  // property and return the largest configured row height
  // This is used to ensure that all cells in a row are
  // aligned when printing the column
  size_t get_configured_height() {
    size_t result{0};
    for (size_t i = 0; i < size(); ++i) {
      auto cell = cells_[i];
      auto format = cell->format();
      if (format.height_.has_value())
        result = std::max(result, format.height_.value());
    }
    return result;
  }

  // Computes the height of the row based on cell contents
  // and configured cell padding
  // For each cell, compute:
  //   padding_top + (cell_contents / column height) + padding_bottom
  // and return the largest value
  //
  // This is useful when no cell.format.height is configured
  // Call get_configured_height()
  // - If this returns 0, then use get_computed_height()
  size_t get_computed_height(const std::vector<size_t> &column_widths) {
    size_t result{0};
    for (size_t i = 0; i < size(); ++i) {
      result = std::max(result, get_cell_height(i, column_widths[i]));
    }
    return result;
  }

  // Returns padding_top + cell_contents / column_height + padding_bottom
  // for a given cell in the column
  // e.g.,
  // column width = 5
  // cell_contents = "I love tabulate" (size/length = 15)
  // padding top and padding bottom are 1
  // then, cell height = 1 + (15 / 5) + 1 = 1 + 3 + 1 = 5
  // The cell will look like this:
  //
  // .....
  // I lov
  // e tab
  // ulate
  // .....
  size_t get_cell_height(size_t cell_index, size_t column_width) {
    size_t result{0};
    Cell &cell = *(cells_[cell_index]);
    auto format = cell.format();
    auto text = cell.get_text();

    result += format.padding_top_.value();

    // Check if input text has embedded newline characters
    auto newlines_in_text = Format::split_lines(text, "\n").size() - 1;
    std::string word_wrapped_text;
    if (newlines_in_text == 0) {
      // No new lines in input
      // Apply automatic word wrapping and compute row height
      word_wrapped_text = Format::word_wrap(text, column_width);
    } else {
      // There are embedded '\n' characters
      // Respect these characters
      word_wrapped_text = text;
    }
    result += std::count(word_wrapped_text.begin(), word_wrapped_text.end(), '\n') + 1;

    result += format.padding_bottom_.value();

    return result;
  }

  std::vector<std::shared_ptr<Cell>> cells_;
  std::weak_ptr<class TableInternal> parent_;
  std::optional<Format> format_;
};

} // namespace tabulate
