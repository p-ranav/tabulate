#pragma once
#include <utility>
#include <vector>
#include <tabulate/color.hpp>

namespace tabulate {

class Printer {
public:
  static std::pair<std::vector<size_t>, std::vector<size_t>>
  compute_cell_dimensions(TableInternal &table);

  static void print_table(std::ostream &stream, TableInternal &table);

  static void print_row_in_cell(std::ostream &stream, TableInternal &table,
                                const std::pair<size_t, size_t> &index,
                                const std::pair<size_t, size_t> &dimension, size_t num_columns,
                                size_t row_index);

  static void print_cell_border_top(std::ostream &stream, TableInternal &table,
                                    const std::pair<size_t, size_t> &index,
                                    const std::pair<size_t, size_t> &dimension, size_t num_columns);
  static void print_cell_border_bottom(std::ostream &stream, TableInternal &table,
                                       const std::pair<size_t, size_t> &index,
                                       const std::pair<size_t, size_t> &dimension, size_t num_columns);

  static void apply_color(std::ostream &stream, Color color) {
    switch (color) {
    case Color::grey:
      stream << termcolor::grey;
      break;
    case Color::red:
      stream << termcolor::red;
      break;
    case Color::green:
      stream << termcolor::green;
      break;
    case Color::yellow:
      stream << termcolor::yellow;
      break;
    case Color::blue:
      stream << termcolor::blue;
      break;
    case Color::magenta:
      stream << termcolor::magenta;
      break;
    case Color::cyan:
      stream << termcolor::cyan;
      break;
    case Color::white:
      stream << termcolor::white;
      break;
    case Color::none:
    default:
      break;
    }
  }

  static void apply_background_color(std::ostream& stream, Color background_color) {
    switch (background_color) {
    case Color::grey:
      stream << termcolor::on_grey;
      break;
    case Color::red:
      stream << termcolor::on_red;
      break;
    case Color::green:
      stream << termcolor::on_green;
      break;
    case Color::yellow:
      stream << termcolor::on_yellow;
      break;
    case Color::blue:
      stream << termcolor::on_blue;
      break;
    case Color::magenta:
      stream << termcolor::on_magenta;
      break;
    case Color::cyan:
      stream << termcolor::on_cyan;
      break;
    case Color::white:
      stream << termcolor::on_white;
      break;
    case Color::none:
    default:
      break;
    }
  }

};

} // namespace tabulate