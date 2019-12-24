#include <algorithm>
#include <iostream>
#include <string>
#include <tabulate/column.hpp>
#include <tabulate/font_style.hpp>
#include <tabulate/printer.hpp>
#include <tabulate/row.hpp>
#include <tabulate/termcolor.hpp>
#include <vector>

namespace tabulate {

Format &Cell::format() {
  std::shared_ptr<Row> parent = parent_.lock();
  if (!format_.has_value()) {   // no cell format
    format_ = parent->format(); // Use parent row format
  } else {
    // Cell has formatting
    // Merge cell formatting with parent row formatting
    format_ = Format::merge(format_.value(), parent->format());
  }
  return format_.value();
}

class TableInternal : public std::enable_shared_from_this<TableInternal> {
public:
  static std::shared_ptr<TableInternal> create() {
    auto result = std::shared_ptr<TableInternal>(new TableInternal());
    result->format_.set_defaults();
    return result;
  }

  void add_row(const std::vector<std::string> &cells) {
    auto row = std::make_shared<Row>(shared_from_this());
    for (auto &c : cells) {
      auto cell = std::make_shared<Cell>(row);
      cell->set_text(c);
      row->add_cell(cell);
    }
    rows_.push_back(row);
  }

  Row &operator[](size_t index) { return *(rows_[index]); }

  const Row &operator[](size_t index) const { return *(rows_[index]); }

  Column column(size_t index) {
    Column column(shared_from_this());
    for (size_t i = 0; i < rows_.size(); ++i) {
      auto row = rows_[i];
      auto &cell = row->cell(index);
      column.add_cell(cell);
    }
    return column;
  }

  size_t size() const { return rows_.size(); }

  Format &format() { return format_; }

  void print(std::ostream &stream) { Printer::print_table(stream, *this); }

  size_t estimate_num_columns() const {
    size_t result{0};
    if (size()) {
      auto first_row = operator[](size_t(0));
      result = first_row.size();
    }
    return result;
  }

private:
  TableInternal() {}
  TableInternal &operator=(const TableInternal &);
  TableInternal(const TableInternal &);

  std::vector<std::shared_ptr<Row>> rows_;
  Format format_;
};

Format &Row::format() {
  std::shared_ptr<TableInternal> parent = parent_.lock();
  if (!format_.has_value()) {   // no row format
    format_ = parent->format(); // Use parent table format
  } else {
    // Row has formatting rules
    // Merge with parent table format
    format_ = Format::merge(format_.value(), parent->format());
  }
  return format_.value();
}

std::pair<std::vector<size_t>, std::vector<size_t>>
Printer::compute_cell_dimensions(TableInternal &table) {
  std::pair<std::vector<size_t>, std::vector<size_t>> result;
  size_t num_rows = table.size();
  size_t num_columns = table.estimate_num_columns();

  std::vector<size_t> row_heights, column_widths{};

  for (size_t i = 0; i < num_columns; ++i) {
    Column column = table.column(i);
    size_t configured_width = column.get_configured_width();
    size_t computed_width = column.get_computed_width();
    if (configured_width != 0)
      column_widths.push_back(configured_width);
    else
      column_widths.push_back(computed_width);
  }

  for (size_t i = 0; i < num_rows; ++i) {
    Row row = table[i];
    size_t configured_height = row.get_configured_height();
    size_t computed_height = row.get_computed_height(column_widths);

    // NOTE: Unlike column width, row height is calculated as the max
    // b/w configured height and computed height
    // which means that .width() has higher precedence than .height()
    // when both are configured by the user
    //
    // TODO: Maybe this can be configured?
    // If such a configuration is exposed, i.e., prefer height over width
    // then the logic will be reversed, i.e.,
    // column_widths.push_back(std::max(configured_width, computed_width))
    // and
    // row_height = configured_height if != 0 else computed_height

    row_heights.push_back(std::max(configured_height, computed_height));
  }

  result.first = row_heights;
  result.second = column_widths;

  return result;
}

void Printer::print_table(std::ostream &stream, TableInternal &table) {
  size_t num_rows = table.size();
  size_t num_columns = table.estimate_num_columns();
  auto dimensions = compute_cell_dimensions(table);
  auto row_heights = dimensions.first;
  auto column_widths = dimensions.second;

  // For each row,
  for (size_t i = 0; i < num_rows; ++i) {

    // Print top border
    bool border_top_printed{true};
    for (size_t j = 0; j < num_columns; ++j) {
      border_top_printed &= print_cell_border_top(stream, table, {i, j}, {row_heights[i], column_widths[j]}, num_columns);
    }
    if (border_top_printed)
      stream << "\n";

    // Print row contents with word wrapping
    for (size_t k = 0; k < row_heights[i]; ++k) {
      for (size_t j = 0; j < num_columns; ++j) {
        print_row_in_cell(stream, table, {i, j}, {row_heights[i], column_widths[j]}, num_columns,
                          k);
      }
      if (k + 1 < row_heights[i])
        stream << "\n";
    }

    if (i + 1 == num_rows) {

      // Check if there is bottom border to print:
      auto bottom_border_needed{true};
      for (size_t j = 0; j < num_columns; ++j) {
        auto cell = table[i][j];
        auto format = cell.format();
        auto column_width = column_widths[j];
        auto corner = format.corner_.value();
        auto border_bottom = format.border_bottom_.value();
        if (corner == "" && border_bottom == "") {
          bottom_border_needed = false;
          break;
        }
      }

      if (bottom_border_needed)
        stream << "\n";
      // Print bottom border for table
      for (size_t j = 0; j < num_columns; ++j) {
        print_cell_border_bottom(stream, table, {i, j}, {row_heights[i], column_widths[j]},
                                 num_columns);
      }
    }
    if (i + 1 < num_rows)
      stream << "\n"; // Don't add newline after last row
  }
}

void Printer::print_row_in_cell(std::ostream &stream, TableInternal &table,
                                const std::pair<size_t, size_t> &index,
                                const std::pair<size_t, size_t> &dimension, size_t num_columns,
                                size_t row_index) {
  auto row_height = dimension.first;
  auto column_width = dimension.second;
  auto cell = table[index.first][index.second];
  auto format = cell.format();
  auto text = cell.get_text();
  auto word_wrapped_text = Format::word_wrap(text, column_width);
  auto text_height = std::count(word_wrapped_text.begin(), word_wrapped_text.end(), '\n') + 1;
  auto padding_top = format.padding_top_.value();
  auto padding_bottom = format.padding_bottom_.value();

  apply_element_style(stream, format.border_left_color_.value(),
                      format.border_left_background_color_.value(), {});
  stream << format.border_left_.value();
  reset_element_style(stream);

  apply_element_style(stream, format.font_color_.value(), format.font_background_color_.value(),
                      {});

  if (row_index < padding_top) {
    // Padding top
    stream << std::string(column_width, ' ');
  } else if (row_index >= padding_top && (row_index <= (padding_top + text_height))) {
    // Row contents

    // Retrieve padding left and right
    // (column_width - padding_left - padding_right) is the amount of space
    // available for cell text - Use this to word wrap cell contents
    auto padding_left = format.padding_left_.value();
    auto padding_right = format.padding_right_.value();

    // Check if input text has embedded \n that are to be respected
    auto newlines_in_input = Format::split_lines(text, "\n").size() - 1;
    std::string word_wrapped_text;

    // If there are no embedded \n characters, then apply word wrap
    if (newlines_in_input == 0) {
      // Apply word wrapping to input text
      // Then display one word-wrapped line at a time within cell
      if (column_width > (padding_left + padding_right))
        word_wrapped_text = Format::word_wrap(text, column_width - padding_left - padding_right);
      else {
        // Configured column width cannot be lower than (padding_left + padding_right)
        // This is a bad configuration
        // E.g., the user is trying to force the column width to be 5
        // when padding_left and padding_right are each configured to 3
        // (padding_left + padding_right) = 6 > column_width
      }
    } else {
      word_wrapped_text = text; // repect the embedded '\n' characters
    }

    auto lines = Format::split_lines(word_wrapped_text, "\n");

    if (row_index - padding_top < lines.size()) {
      auto line = lines[row_index - padding_top];

      // Print left padding characters
      stream << std::string(padding_left, ' ');

      // Print word-wrapped line
      line = Format::trim(line);
      auto line_with_padding_size = line.size() + padding_left + padding_right;
      switch (format.font_align_.value()) {
      case FontAlign::left:
        print_content_left_aligned(stream, line, format, line_with_padding_size, column_width);
        break;
      case FontAlign::center:
        print_content_center_aligned(stream, line, format, line_with_padding_size, column_width);
        break;
      case FontAlign::right:
        print_content_right_aligned(stream, line, format, line_with_padding_size, column_width);
        break;
      }

      // Print right padding characters
      stream << std::string(format.padding_right_.value(), ' ');
    } else
      stream << std::string(column_width, ' ');

  } else {
    // Padding bottom
    stream << std::string(column_width, ' ');
  }

  reset_element_style(stream);

  if (index.second + 1 == num_columns) {
    // Print right border after last column
    apply_element_style(stream, format.border_right_color_.value(),
                        format.border_right_background_color_.value(), {});
    stream << format.border_right_.value();
    reset_element_style(stream);
  }
}

bool Printer::print_cell_border_top(std::ostream &stream, TableInternal &table,
                                    const std::pair<size_t, size_t> &index,
                                    const std::pair<size_t, size_t> &dimension,
                                    size_t num_columns) {
  auto cell = table[index.first][index.second];
  auto format = cell.format();
  auto column_width = dimension.second;

  auto corner = format.corner_.value();
  auto border_top = format.border_top_.value();

  if (corner == "" && border_top == "")
    return false;

  apply_element_style(stream, format.corner_color_.value(), format.corner_background_color_.value(),
                      {});
  stream << corner;
  reset_element_style(stream);

  for (size_t i = 0; i < column_width; ++i) {
    apply_element_style(stream, format.border_top_color_.value(),
                        format.border_bottom_background_color_.value(), {});
    stream << border_top;
    reset_element_style(stream);
  }

  if (index.second + 1 == num_columns) {
    // Print corner after last column
    apply_element_style(stream, format.corner_color_.value(),
                        format.corner_background_color_.value(), {});
    stream << corner;
    reset_element_style(stream);
  }
  return true;
}

bool Printer::print_cell_border_bottom(std::ostream &stream, TableInternal &table,
                                       const std::pair<size_t, size_t> &index,
                                       const std::pair<size_t, size_t> &dimension,
                                       size_t num_columns) {
  auto cell = table[index.first][index.second];
  auto format = cell.format();
  auto column_width = dimension.second;

  auto corner = format.corner_.value();
  auto border_bottom = format.border_bottom_.value();

  if (corner == "" && border_bottom == "")
    return false;

  apply_element_style(stream, format.corner_color_.value(), format.corner_background_color_.value(),
                      {});
  stream << corner;
  reset_element_style(stream);

  for (size_t i = 0; i < column_width; ++i) {
    apply_element_style(stream, format.border_bottom_color_.value(),
                        format.border_left_background_color_.value(), {});
    stream << border_bottom;
    reset_element_style(stream);
  }

  if (index.second + 1 == num_columns) {
    // Print corner after last column
    apply_element_style(stream, format.corner_color_.value(),
                        format.corner_background_color_.value(), {});
    stream << corner;
    reset_element_style(stream);
  }
  return true;
}

} // namespace tabulate