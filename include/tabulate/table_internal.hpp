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
  if (!format_.has_value()) { // no cell format
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
  if (!format_.has_value()) { // no row format
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
    for (size_t j = 0; j < num_columns; ++j) {
      print_cell_border_top(stream, table, {i, j}, {row_heights[i], column_widths[j]}, num_columns);
    }
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
      stream << "\n";
      // Print bottom border for table
      for (size_t j = 0; j < num_columns; ++j) {
        print_cell_border_bottom(stream, table, {i, j}, {row_heights[i], column_widths[j]}, num_columns);
      }
    }

    std::cout << "\n";
  }
}

void Printer::print_row_in_cell(std::ostream &stream, TableInternal &table,
                                const std::pair<size_t, size_t> &index,
                                const std::pair<size_t, size_t> &dimension, size_t num_columns,
                                size_t row_index) {
  auto cell = table[index.first][index.second];
  auto format = cell.format();
  auto text = cell.get_text();
  auto text_size = text.size();
  auto text_with_padding_size =
      format.padding_left_.value() + text_size + format.padding_right_.value();
  auto row_height = dimension.first;
  auto column_width = dimension.second;
  auto padding_top = format.padding_top_.value();
  auto padding_bottom = format.padding_bottom_.value();

  stream << format.border_left_.value();

  if (row_index < padding_top) {
    // Padding top
    stream << std::string(column_width, ' ');
  } else if (row_index >= padding_top &&
             (row_index <= (padding_top + text_with_padding_size / column_width))) {
    // Row contents
    if (column_width >= text_with_padding_size) {
      row_index -= padding_top;
      if (row_index * column_width < text.size()) {
        stream << std::string(format.padding_left_.value(), ' ') << text
                << std::string(format.padding_right_.value(), ' ')
                << std::string(column_width - text_with_padding_size, ' ');
      } else {
        stream << std::string(column_width, ' ');
      }
    } else {
      // Multiple rows required for this cell
      auto text_section_width =
          column_width - format.padding_left_.value() - format.padding_right_.value();
      row_index -= padding_top;
      if (row_index * text_section_width < text_size) {
        auto sub_string = text.substr(row_index * text_section_width, text_section_width);
        stream << std::string(format.padding_left_.value(), ' ') << sub_string;
        if (column_width >
            sub_string.size() + format.padding_left_.value() + format.padding_right_.value())
          stream << std::string(column_width - sub_string.size() - format.padding_left_.value() -
                                    format.padding_right_.value(),
                                ' ');
        stream << std::string(format.padding_right_.value(), ' ');
      }
    }
  } else {
    // Padding bottom
    stream << std::string(column_width, ' ');
  }

  if (index.second + 1 == num_columns) {
    // Print right border after last column
    stream << format.border_right_.value();
  }
}

void Printer::print_cell_border_top(std::ostream &stream, TableInternal &table,
                                    const std::pair<size_t, size_t> &index,
                                    const std::pair<size_t, size_t> &dimension, 
                                    size_t num_columns) {
  auto cell = table[index.first][index.second];
  auto format = cell.format();
  auto column_width = dimension.second;

  auto corner = format.corner_.value();
  auto border_top = format.border_top_.value();
  stream << corner;
  for (size_t i = 0; i < column_width; ++i)
    stream << border_top;
  if (index.second + 1 == num_columns) {
    // Print corner after last column
    stream << format.corner_.value();
  }
}

void Printer::print_cell_border_bottom(std::ostream &stream, TableInternal &table,
                                       const std::pair<size_t, size_t> &index,
                                       const std::pair<size_t, size_t> &dimension, 
                                       size_t num_columns) {
  auto cell = table[index.first][index.second];
  auto format = cell.format();
  auto column_width = dimension.second;

  auto corner = format.corner_.value();
  auto border_bottom = format.border_bottom_.value();
  stream << corner;
  for (size_t i = 0; i < column_width; ++i)
    stream << border_bottom;
  if (index.second + 1 == num_columns) {
    // Print corner after last column
    stream << format.corner_.value();
  }
}

} // namespace tabulate