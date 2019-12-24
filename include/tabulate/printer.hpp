#pragma once
#include <utility>
#include <vector>
#include <tabulate/color.hpp>
#include <tabulate/font_style.hpp>

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

  static void apply_element_style(std::ostream &stream, Color foreground_color, Color background_color, const std::vector<FontStyle>& font_style) {
    apply_foreground_color(stream, foreground_color);
    apply_background_color(stream, background_color);
    for (auto& style: font_style)
      apply_font_style(stream, style);
  }

  static void reset_element_style(std::ostream &stream) { stream << termcolor::reset; }

private:

  static void apply_font_style(std::ostream& stream, FontStyle style) {
    switch (style) {
    case FontStyle::bold:
      stream << termcolor::bold;
      break;
    case FontStyle::dark:
      stream << termcolor::dark;
      break;
    case FontStyle::italic:
      stream << termcolor::italic;
      break;
    case FontStyle::underline:
      stream << termcolor::underline;
      break;
    case FontStyle::blink:
      stream << termcolor::blink;
      break;
    case FontStyle::reverse:
      stream << termcolor::reverse;
      break;
    case FontStyle::concealed:
      stream << termcolor::concealed;
      break;
    case FontStyle::crossed:
      stream << termcolor::crossed;
      break;
    default:
      break;
    }
  }

  static void apply_foreground_color(std::ostream &stream, Color foreground_color) {
    switch (foreground_color) {
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