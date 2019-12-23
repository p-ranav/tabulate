#include <iostream>
#include <string>
#include <tabulate/font_style.hpp>
#include <tabulate/row.hpp>
#include <tabulate/termcolor.hpp>
#include <vector>

namespace tabulate {

Format &Cell::format() {
  if (!format_.has_value()) {         // no cell format
    format_ = parent_.get().format(); // Use parent row format
  }
  return format_.value();
}

class Table {
public:
  void add_row(const std::vector<std::string> &cells) {
    Row row(*this);
    for (auto &c : cells) {
      auto cell = Cell(row);
      cell.data_ = c;
      row.add_cell(std::move(cell));
    }
    rows_.push_back(std::move(row));
  }

  Row &operator[](size_t index) { return rows_[index]; }

  Format &format() { return format_; }

private:
  friend std::ostream &operator<<(std::ostream &os, const Table &table);

  void print(std::ostream &stream = std::cout) const {
    for (size_t i = 0; i < rows_.size(); ++i) {
      auto &row = rows_[i];
      auto row_format = row.format_;
      auto row_height = get_row_height(i);
      const auto &cells = row.cells();

      // Header row
      auto header_printed_{false};
      for (size_t j = 0; j < cells.size(); ++j) {
        header_printed_ = print_cell_header(stream, i, j);
      }
      if (header_printed_)
        stream << "\n";

      // Padding top
      Format format = get_format(row_format, {});
      for (size_t k = 0; k < format.padding_top_; ++k) {
        print_padding_row(stream, i);
        stream << "\n";
      }

      // Row contents
      for (size_t k = 0; k < row_height; ++k) {
        std::vector<std::string> sub_row_contents;
        std::vector<size_t> column_widths;
        for (size_t l = 0; l < cells.size(); ++l) {
          auto column_width = get_column_width(l);
          auto cell_contents = cells[l].data_;
          auto pos = k * column_width;
          auto size = cell_contents.size();
          if (pos < size) {
            auto remaining = (size - pos);
            sub_row_contents.push_back(
                cell_contents.substr(pos, std::min(remaining, column_width)));
          } else {
            sub_row_contents.push_back("");
          }
          column_widths.push_back(column_width);
        }
        print_content_row(stream, i, sub_row_contents, column_widths);
      }

      // Padding bottom
      for (size_t k = 0; k < format.padding_bottom_; ++k) {
        print_padding_row(stream, i);
        stream << "\n";
      }

      // Footer row
      if (i + 1 == rows_.size()) {
        for (size_t j = 0; j < cells.size(); ++j) {
          print_cell_footer(stream, i, j);
        }
      }
    }
  }

  void apply_element_style(std::ostream &stream, Format format) const {
    reset_style(stream);
    auto font_style = format.font_style_;
    for (auto &style : font_style) {
      apply_font_style(stream, style);
    }

    auto color = format.font_color_;
    if (color.has_value()) {
      apply_color(stream, color.value());
    }

    auto background_color = format.font_background_color_;
    if (background_color.has_value()) {
      apply_background_color(stream, background_color.value());
    }
  }

  void apply_font_style(std::ostream &stream, FontStyle style) const {
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

  void apply_color(std::ostream &stream, Color color) const {
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
    }
  }

  void apply_background_color(std::ostream &stream, Color background_color) const {
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
    }
  }

  void reset_style(std::ostream &stream) const { stream << termcolor::reset; }

  size_t get_column_width(size_t index) const {
    size_t result{0};
    for (auto &row : rows_) {
      if (index < row.size()) {
        auto cell = row.get_cell(index);
        size_t cell_width = cell.has_value() ? cell.value().size() : 0;
        if (format_.width_.has_value()) {
          result = format_.width_.value();
        } else {
          result = std::max(result, cell_width);
        }
      }
    }
    return result;
  }

  size_t get_row_height(size_t index) const {
    size_t result{1};
    if (index < rows_.size()) {
      result = std::max(result, rows_[index].height());
    }
    return result;
  }

  void print_padding_row(std::ostream &stream, size_t row_index) const {
    Format format;
    auto row = rows_[row_index];
    auto row_format = row.format_;
    for (size_t col_index = 0; col_index < row.size(); ++col_index) {
      auto cell = row[col_index];
      auto cell_format = cell.format_;

      format = get_format(row_format, cell_format);

      auto width = get_column_width(col_index);

      // add padding to width
      width += format.padding_left_;
      width += format.padding_right_;

      if (col_index == 0) {
        if (format.border_left_color_.has_value()) {
          apply_color(stream, format.border_left_color_.value());
        }
        if (format.border_left_background_color_.has_value()) {
          apply_background_color(stream, format.border_left_background_color_.value());
        }
        stream << format.border_left_;
        reset_style(stream);
      } else {
        if (format.column_separator_color_.has_value()) {
          apply_color(stream, format.column_separator_color_.value());
        }
        if (format.column_separator_background_color_.has_value()) {
          apply_background_color(stream, format.column_separator_background_color_.value());
        }
        stream << format.column_separator_;
        reset_style(stream);
      }

      apply_element_style(stream, format);

      size_t i = 0;
      while (i < width) {
        stream << " ";
        ++i;
      }
      reset_style(stream);
    }

    if (format.border_right_color_.has_value()) {
      apply_color(stream, format.border_right_color_.value());
    }
    if (format.border_right_background_color_.has_value()) {
      apply_background_color(stream, format.border_right_background_color_.value());
    }
    stream << format.border_right_;
    reset_style(stream);
  }

  void print_content_row(std::ostream &stream, size_t row_index,
                         std::vector<std::string> row_contents,
                         std::vector<size_t> column_widths) const {
    Format format;
    auto row = rows_[row_index];
    auto row_format = row.format_;
    for (size_t i = 0; i < row_contents.size(); ++i) {
      auto cell = row[i];
      auto cell_format = cell.format_;

      format = get_format(row_format, cell_format);

      auto cell_content = row_contents[i];

      if (i == 0) {
        if (format.border_left_color_.has_value()) {
          apply_color(stream, format.border_left_color_.value());
        }
        if (format.border_left_background_color_.has_value()) {
          apply_background_color(stream, format.border_left_background_color_.value());
        }
        stream << format.border_left_;
        reset_style(stream);
      } else {
        if (format.column_separator_color_.has_value()) {
          apply_color(stream, format.column_separator_color_.value());
        }
        if (format.column_separator_background_color_.has_value()) {
          apply_background_color(stream, format.column_separator_background_color_.value());
        }
        stream << format.column_separator_;
        reset_style(stream);
      }

      apply_element_style(stream, format);
      for (size_t j = 0; j < format.padding_left_; ++j) {
        stream << " ";
      }

      switch (format.font_align_) {
      case FontAlign::left:
        print_content_left_aligned(stream, cell_content, column_widths[i]);
        break;
      case FontAlign::center:
        print_content_center_aligned(stream, cell_content, column_widths[i]);
        break;
      case FontAlign::right:
        print_content_right_aligned(stream, cell_content, column_widths[i]);
        break;
      }

      for (size_t j = 0; j < format.padding_right_; ++j) {
        stream << " ";
      }
      reset_style(stream);
    }

    if (format.border_right_color_.has_value()) {
      apply_color(stream, format.border_right_color_.value());
    }
    if (format.border_right_background_color_.has_value()) {
      apply_background_color(stream, format.border_right_background_color_.value());
    }
    stream << format.border_right_;
    reset_style(stream);

    if (row_index + 1 < rows_.size()) {
      stream << "\n";
    } else {
      // Last row. Print newline after last content row?
      // Yes ONLY if there's a padding bottom or border bottom
      if (format.padding_bottom_ != 0 || format.border_bottom_ != "") {
        stream << "\n";
      }
    }
  }

  void print_content_left_aligned(std::ostream &stream, std::string cell_content,
                                  size_t column_width) const {
    stream << cell_content;
    auto content_width = cell_content.size();
    if (content_width < column_width) {
      for (size_t j = 0; j < (column_width - content_width); ++j) {
        stream << " ";
      }
    }
  }

  void print_content_center_aligned(std::ostream &stream, std::string cell_content,
                                    size_t column_width) const {
    auto content_width = cell_content.size();
    auto num_spaces = column_width - content_width;
    if (num_spaces % 2 == 0) {
      // Even spacing on either side
      for (size_t j = 0; j < num_spaces / 2; ++j)
        stream << " ";
      stream << cell_content;
      for (size_t j = 0; j < num_spaces / 2; ++j)
        stream << " ";
    } else {
      auto num_spaces_before = num_spaces / 2 + 1;
      for (size_t j = 0; j < num_spaces_before; ++j)
        stream << " ";
      stream << cell_content;
      for (size_t j = 0; j < num_spaces - num_spaces_before; ++j)
        stream << " ";
    }
  }

  void print_content_right_aligned(std::ostream &stream, std::string cell_content,
                                   size_t column_width) const {
    auto content_width = cell_content.size();
    if (content_width < column_width) {
      for (size_t j = 0; j < (column_width - content_width); ++j) {
        stream << " ";
      }
    }
    stream << cell_content;
  }

  bool print_cell_header(std::ostream &stream, size_t row_index, size_t col_index) const {
    auto row = rows_[row_index];
    auto row_format = row.format_;
    auto cell_format = row[col_index].format_;
    Format format = get_format(row_format, cell_format);

    if (format.corner_ == "" && format.border_top_ == "")
      return false; // don't print header

    auto width = get_column_width(col_index);
    // add padding to width
    width += format.padding_left_;
    width += format.padding_right_;

    if (col_index == 0) {
      if (format.corner_color_.has_value()) {
        apply_color(stream, format.corner_color_.value());
      }
      if (format.corner_background_color_.has_value()) {
        apply_background_color(stream, format.corner_background_color_.value());
      }
      stream << format.corner_;
      reset_style(stream);
    }

    if (format.border_top_ != "") {
      size_t i = 0;
      while (i < width) {
        // Apply border_top format
        if (format.border_top_color_.has_value()) {
          apply_color(stream, format.border_top_color_.value());
        }
        if (format.border_top_background_color_.has_value()) {
          apply_background_color(stream, format.border_top_background_color_.value());
        }
        stream << format.border_top_;
        reset_style(stream);
        ++i;
      }
    }

    if (format.corner_color_.has_value()) {
      apply_color(stream, format.corner_color_.value());
    }
    if (format.corner_background_color_.has_value()) {
      apply_background_color(stream, format.corner_background_color_.value());
    }
    stream << format.corner_;
    reset_style(stream);
    return true;
  }

  bool print_cell_footer(std::ostream &stream, size_t row_index, size_t col_index) const {
    auto row = rows_[row_index];
    auto row_format = row.format_;
    auto cell_format = row[col_index].format_;
    Format format = get_format(row_format, cell_format);

    if (format.corner_ == "" && format.border_bottom_ == "")
      return false; // don't print header

    auto width = get_column_width(col_index);
    // add padding to width
    width += format.padding_left_;
    width += format.padding_right_;

    if (col_index == 0) {
      if (format.corner_color_.has_value()) {
        apply_color(stream, format.corner_color_.value());
      }
      if (format.corner_background_color_.has_value()) {
        apply_background_color(stream, format.corner_background_color_.value());
      }
      stream << format.corner_;
      reset_style(stream);
    }

    if (format.border_bottom_ != "") {
      size_t i = 0;
      while (i < width) {
        if (format.border_bottom_color_.has_value()) {
          apply_color(stream, format.border_bottom_color_.value());
        }
        if (format.border_bottom_background_color_.has_value()) {
          apply_background_color(stream, format.border_bottom_background_color_.value());
        }
        stream << format.border_bottom_;
        reset_style(stream);
        ++i;
      }
    }

    if (format.corner_color_.has_value()) {
      apply_color(stream, format.corner_color_.value());
    }
    if (format.corner_background_color_.has_value()) {
      apply_background_color(stream, format.corner_background_color_.value());
    }
    stream << format.corner_;
    reset_style(stream);
    return true;
  }

  Format get_format(std::optional<Format> row_format, std::optional<Format> cell_format) const {
    Format result;
    // Check for cell-level formatting override
    // This format takes preference
    if (cell_format.has_value()) {
      result = cell_format.value();
    }
    // No cell formatting override
    // Check for row-level formatting
    else if (row_format.has_value()) {
      result = row_format.value();
    }
    // No cell or row formatting overrides
    // Use table-level formatting
    else {
      result = format_;
    }
    return result;
  }

  std::vector<Row> rows_;
  Format format_;
};

Format &Row::format() {
  if (!format_.has_value()) {      // no row format
    return parent_.get().format(); // Use parent table format
  }
  return format_.value();
}

std::ostream &operator<<(std::ostream &os, const Table &table) {
  table.print(os);
  return os;
}

} // namespace tabulate
