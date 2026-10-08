
/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
#include <algorithm>
#include <iostream>
#include <string>
#include <tabulate/column.hpp>
#include <tabulate/font_style.hpp>
#include <tabulate/printer.hpp>
#include <tabulate/row.hpp>
#include <tabulate/termcolor.hpp>
#include <vector>
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

namespace tabulate {

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

  void erase_column(size_t index) {
    for (auto &row : rows_) {
      row->erase_cell(index);
    }
  }

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

  // {rendered width in characters, rendered height in lines} of the printed
  // table -- NOT the number of data rows/columns. See Table::dimensions()
  // for that.
  std::pair<size_t, size_t> shape() {
    std::pair<size_t, size_t> result{0, 0};
    std::stringstream stream;
    print(stream);
    auto buffer = stream.str();
    auto lines = Format::split_lines(buffer, "\n", "", true);
    if (lines.size()) {
      result = {get_sequence_length(lines[0], "", true), lines.size()};
    }
    return result;
  }

  Format &format() { return format_; }

  void print(std::ostream &stream) { Printer::print_table(stream, *this); }

  // Prints one row (its own top border, acting as a separator, plus content)
  // without the table's closing bottom border, so rows can be streamed to a
  // terminal/log as add_row() is called instead of printing the whole table
  // once at the end. Call print_bottom_border() once after the last row.
  //
  // Column widths are only stable across separate print_row() calls if
  // fixed up front via column(i).format().width(n); otherwise a later, wider
  // row can retroactively invalidate the alignment of rows already printed.
  void print_row(std::ostream &stream, size_t row_index) {
    auto dimensions = Printer::compute_cell_dimensions(*this);
    auto &row_heights = dimensions.first;
    auto &column_widths = dimensions.second;
    size_t num_columns = estimate_num_columns();

    Row &row = operator[](row_index);
    std::vector<std::vector<std::string>> splitted_cells_text(num_columns);
    for (size_t j = 0; j < num_columns; ++j) {
      Cell &cell = row.cell(j);
      splitted_cells_text[j] = Printer::split_cell_text(cell, column_widths[j]);
    }

    bool border_top_printed = true;
    for (size_t j = 0; j < num_columns; ++j) {
      border_top_printed &= Printer::print_cell_border_top(
          stream, *this, {row_index, j}, {row_heights[row_index], column_widths[j]}, num_columns);
    }
    if (border_top_printed)
      stream << termcolor::reset << "\n";

    for (size_t k = 0; k < row_heights[row_index]; ++k) {
      for (size_t j = 0; j < num_columns; ++j) {
        Printer::print_row_in_cell(stream, *this, {row_index, j},
                                   {row_heights[row_index], column_widths[j]}, num_columns, k,
                                   splitted_cells_text[j]);
      }
      stream << termcolor::reset << "\n";
    }
  }

  // Prints the table's closing bottom border, using the last row's format
  // for the corner/border characters. Pair with print_row().
  void print_bottom_border(std::ostream &stream) {
    auto dimensions = Printer::compute_cell_dimensions(*this);
    auto &row_heights = dimensions.first;
    auto &column_widths = dimensions.second;
    size_t num_columns = estimate_num_columns();
    size_t last_row = size() - 1;

    for (size_t j = 0; j < num_columns; ++j) {
      Printer::print_cell_border_bottom(stream, *this, {last_row, j},
                                        {row_heights[last_row], column_widths[j]}, num_columns);
    }
    stream << termcolor::reset << "\n";
  }

  size_t estimate_num_columns() const {
    size_t result{0};
    if (size()) {
      auto first_row = operator[](size_t(0));
      result = first_row.size();
    }
    return result;
  }

private:
  friend class Table;
  friend class MarkdownExporter;

  TableInternal() {}
  TableInternal &operator=(const TableInternal &);
  TableInternal(const TableInternal &);

  std::vector<std::shared_ptr<Row>> rows_;
  Format format_;
};

inline Format &Cell::format() {
  // A reference, not a copy: Format has ~50 optional<> fields (several
  // strings/vectors), so copying it on every call -- including the common
  // case below where nothing changed and we return the cached result --
  // would dominate print() cost on tables with many cells.
  const Format &parent_format = parent_->format();
  if (format_.has_value() && parent_format_snapshot_.has_value()) {
    if (parent_format.generation() == parent_format_snapshot_->generation())
      // Nothing upstream changed since the last merge (cheap O(1) check):
      // the cached result is still correct, so skip the expensive
      // field-by-field work below.
      return *format_;
    // Let fields nobody explicitly overrode here track the parent's latest format.
    Format::reset_inherited_fields(*format_, *parent_format_snapshot_);
  }
  // merge(Format(), parent_format) always reduces to parent_format field by
  // field (an empty Format has nothing to take precedence) -- skip the ~49
  // conditional field copies and the Format() throwaway when there's
  // nothing of our own to merge in.
  format_ = format_.has_value() ? Format::merge(*format_, parent_format) : parent_format;
  parent_format_snapshot_ = parent_format;
  return *format_;
}

inline bool Cell::is_multi_byte_character_support_enabled() {
  return (*format().multi_byte_characters_);
}

inline Format &Row::format() {
  // See the comment in Cell::format(): a reference avoids an expensive
  // whole-Format copy on every call.
  const Format &parent_format = parent_->format();
  if (format_.has_value() && parent_format_snapshot_.has_value()) {
    if (parent_format.generation() == parent_format_snapshot_->generation())
      // Nothing upstream changed since the last merge (cheap O(1) check):
      // the cached result is still correct, so skip the expensive
      // field-by-field work below.
      return *format_;
    // Let fields nobody explicitly overrode here track the parent's latest format.
    Format::reset_inherited_fields(*format_, *parent_format_snapshot_);
  }
  // See the comment in Cell::format() above for why this skips merge() when
  // there are no row-level overrides to merge in.
  format_ = format_.has_value() ? Format::merge(*format_, parent_format) : parent_format;
  parent_format_snapshot_ = parent_format;
  return *format_;
}

inline std::vector<size_t> Printer::compute_column_widths(TableInternal &table) {
  size_t num_columns = table.estimate_num_columns();
  size_t num_rows = table.size();
  std::vector<size_t> configured_widths(num_columns, 0);
  std::vector<size_t> computed_widths(num_columns, 0);

  // A single direct pass over every cell, each visited exactly once and its
  // format() computed exactly once. table.column(j) would instead rebuild a
  // whole vector<reference_wrapper<Cell>> by re-scanning every row, once per
  // column, then scan it twice more (get_configured_width() and
  // get_computed_width()) -- effectively 3 full O(rows) passes per column
  // where one combined O(rows * columns) pass suffices.
  for (size_t i = 0; i < num_rows; ++i) {
    Row &row = table[i];
    for (size_t j = 0; j < num_columns; ++j) {
      Cell &cell = row.cell(j);
      const auto &format = cell.format();
      if (format.width_.has_value())
        configured_widths[j] = std::max(configured_widths[j], *format.width_);
      computed_widths[j] = std::max(computed_widths[j], Column::get_cell_width(cell));
    }
  }

  std::vector<size_t> column_widths(num_columns);
  for (size_t j = 0; j < num_columns; ++j)
    column_widths[j] = configured_widths[j] != 0 ? configured_widths[j] : computed_widths[j];
  return column_widths;
}

inline std::pair<std::vector<size_t>, std::vector<size_t>>
Printer::compute_cell_dimensions(TableInternal &table) {
  std::pair<std::vector<size_t>, std::vector<size_t>> result;
  size_t num_rows = table.size();
  std::vector<size_t> column_widths = compute_column_widths(table);
  std::vector<size_t> row_heights;
  row_heights.reserve(num_rows);

  for (size_t i = 0; i < num_rows; ++i) {
    Row &row = table[i];
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

inline std::vector<std::string> Printer::split_cell_text(Cell &cell, size_t column_width) {
  const std::string &text = cell.get_text();
  const Format &format = cell.format();
  auto padding_left = *format.padding_left_;
  auto padding_right = *format.padding_right_;

  // Check if input text has embedded \n that are to be respected
  bool has_new_line = text.find_first_of('\n') != std::string::npos;

  if (has_new_line) {
    // Respect to the embedded '\n' characters
    return Format::split_lines(text, "\n", cell.locale(), cell.is_multi_byte_character_support_enabled());
  }

  // If there are no embedded \n characters, then apply word wrap.
  //
  // Configured column width cannot be lower than (padding_left +
  // padding_right) This is a bad configuration E.g., the user is trying
  // to force the column width to be 5 when padding_left and padding_right
  // are each configured to 3 (padding_left + padding_right) = 6 >
  // column_width
  auto content_width = column_width > padding_left + padding_right ? column_width - padding_left - padding_right
                                                                   : column_width;

  // Common case: the column is already at least as wide as this cell's own
  // text (e.g. every column width not explicitly configured is sized to fit
  // its widest cell, so every other/shorter cell in it always fits too).
  // word_wrap() would tokenize the text on spaces/dashes/tabs and then
  // reassemble those tokens completely unchanged -- skip that tokenizing
  // machinery and go straight to the identical single-line result.
  if (text.empty())
    return {};
  if (get_sequence_length(text, cell.locale(), cell.is_multi_byte_character_support_enabled()) <=
      content_width)
    return {text};

  auto word_wrapped_text = Format::word_wrap(text, content_width, cell.locale(),
                                             cell.is_multi_byte_character_support_enabled());
  return Format::split_lines(word_wrapped_text, "\n", cell.locale(),
                             cell.is_multi_byte_character_support_enabled());
}

inline void Printer::print_table(std::ostream &stream, TableInternal &table) {
  size_t indent = *table.format().indent_;
  std::ostringstream buffer_stream;
  if (indent > 0 && termcolor::_internal::is_colorized(stream))
    buffer_stream << termcolor::colorize;
  std::ostream &stream_ref = indent == 0 ? stream : buffer_stream;

  size_t num_rows = table.size();
  size_t num_columns = table.estimate_num_columns();
  std::vector<size_t> column_widths = compute_column_widths(table);
  auto splitted_cells_text = std::vector<std::vector<std::vector<std::string>>>(
      num_rows, std::vector<std::vector<std::string>>(num_columns, std::vector<std::string>{}));
  std::vector<size_t> row_heights(num_rows, 0);

  // Word-wrap each cell's text exactly once, then derive its row's height
  // directly from the resulting line count -- row.get_computed_height()
  // would otherwise redo the exact same word_wrap() independently just to
  // count lines, doubling the cost of what is usually the most expensive
  // part of printing a table.
  for (size_t i = 0; i < num_rows; ++i) {
    Row &row = table[i];
    size_t row_height = row.get_configured_height();
    for (size_t j = 0; j < num_columns; ++j) {
      Cell &cell = row.cell(j);
      auto &lines = splitted_cells_text[i][j];
      lines = split_cell_text(cell, column_widths[j]);
      const auto &format = cell.format();
      // split_cell_text() drops a trailing empty segment (see
      // Format::split_lines), so completely empty cell content comes back
      // as 0 lines -- but it still occupies one content line on screen.
      size_t line_count = lines.empty() ? 1 : lines.size();
      size_t cell_height = *format.padding_top_ + line_count + *format.padding_bottom_;
      row_height = std::max(row_height, cell_height);
    }
    row_heights[i] = row_height;
  }

  // For each row,
  for (size_t i = 0; i < num_rows; ++i) {

    // Print top border
    bool border_top_printed{true};
    for (size_t j = 0; j < num_columns; ++j) {
      border_top_printed &= print_cell_border_top(stream_ref, table, {i, j},
                                                  {row_heights[i], column_widths[j]}, num_columns);
    }
    if (border_top_printed)
      stream_ref << termcolor::reset << "\n";

    // Print row contents with word wrapping
    for (size_t k = 0; k < row_heights[i]; ++k) {
      for (size_t j = 0; j < num_columns; ++j) {
        print_row_in_cell(stream_ref, table, {i, j}, {row_heights[i], column_widths[j]}, num_columns, k,
                          splitted_cells_text[i][j]);
      }
      if (k + 1 < row_heights[i])
        stream_ref << termcolor::reset << "\n";
    }

    if (i + 1 == num_rows) {

      // Check if there is bottom border to print:
      auto bottom_border_needed{true};
      for (size_t j = 0; j < num_columns; ++j) {
        auto &cell = table[i][j];
        const auto &format = cell.format();
        auto corner = *format.corner_bottom_left_;
        auto border_bottom = *format.border_bottom_;
        if (corner == "" && border_bottom == "") {
          bottom_border_needed = false;
          break;
        }
      }

      if (bottom_border_needed)
        stream_ref << termcolor::reset << "\n";
      // Print bottom border for table
      for (size_t j = 0; j < num_columns; ++j) {
        print_cell_border_bottom(stream_ref, table, {i, j}, {row_heights[i], column_widths[j]},
                                 num_columns);
      }
    }
    if (i + 1 < num_rows)
      stream_ref << termcolor::reset << "\n"; // Don't add newline after last row
  }

  if (indent > 0) {
    // Manual split (not Format::split_lines, which drops a trailing empty
    // segment): preserves the exact line structure, including a trailing
    // newline, so indent > 0 and indent == 0 differ only by the padding.
    const std::string content = buffer_stream.str();
    const std::string pad(indent, ' ');
    size_t pos = 0;
    while (pos < content.size()) {
      size_t newline_pos = content.find('\n', pos);
      if (newline_pos == std::string::npos) {
        stream << pad << content.substr(pos);
        break;
      }
      stream << pad << content.substr(pos, newline_pos - pos) << "\n";
      pos = newline_pos + 1;
    }
  }
}

inline void Printer::print_row_in_cell(std::ostream &stream, TableInternal &table,
                                       const std::pair<size_t, size_t> &index,
                                       const std::pair<size_t, size_t> &dimension,
                                       size_t num_columns, size_t row_index,
                                       const std::vector<std::string> &splitted_cell_text) {
  auto column_width = dimension.second;
  auto &cell = table[index.first][index.second];
  const auto &format = cell.format();
  // Avoids the redundant format() lookups that cell.locale()/
  // cell.is_multi_byte_character_support_enabled() would otherwise each do
  // on every one of their several call sites below.
  const std::string &locale = *format.locale_;
  auto is_multi_byte_character_support_enabled = *format.multi_byte_characters_;
  auto text_height = splitted_cell_text.size();
  auto padding_top = *format.padding_top_;

  if (*format.show_border_left_) {
    apply_element_style(stream, *format.border_left_color_, *format.border_left_background_color_,
                        *format.border_left_style_);
    stream << *format.border_left_;
    reset_element_style(stream);
  } else {
    // Keep the row the same width as the border/corner lines around it
    stream << std::string(
        get_sequence_length(*format.border_left_, locale, is_multi_byte_character_support_enabled),
        ' ');
  }

  apply_element_style(stream, *format.font_color_, *format.font_background_color_, {});
  if (row_index < padding_top) {
    // Padding top
    stream << std::string(column_width, ' ');
  } else if (row_index >= padding_top && (row_index <= (padding_top + text_height))) {
    // Retrieve padding left and right
    // (column_width - padding_left - padding_right) is the amount of space
    // available for cell text - Use this to word wrap cell contents
    auto padding_left = *format.padding_left_;
    auto padding_right = *format.padding_right_;

    if (row_index - padding_top < text_height) {
      auto line = splitted_cell_text[row_index - padding_top];

      // Print left padding characters
      stream << std::string(padding_left, ' ');

      // Print word-wrapped line
      switch (*format.trim_mode_) {
      case Format::TrimMode::kBoth:
        line = Format::trim(line);
        break;
      case Format::TrimMode::kLeft:
        line = Format::trim_left(line);
        break;
      case Format::TrimMode::kRight:
        line = Format::trim_right(line);
        break;
      case Format::TrimMode::kNone:
        break;
      }

      auto line_with_padding_size =
          get_sequence_length(line, locale, is_multi_byte_character_support_enabled) +
          padding_left + padding_right;
      switch (*format.font_align_) {
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
      stream << std::string(padding_right, ' ');
    } else
      stream << std::string(column_width, ' ');

  } else {
    // Padding bottom
    stream << std::string(column_width, ' ');
  }

  reset_element_style(stream);

  if (index.second + 1 == num_columns) {
    // Print right border after last column
    if (*format.show_border_right_) {
      apply_element_style(stream, *format.border_right_color_,
                          *format.border_right_background_color_, *format.border_right_style_);
      stream << *format.border_right_;
      reset_element_style(stream);
    } else {
      // Keep the row the same width as the border/corner lines around it
      stream << std::string(get_sequence_length(*format.border_right_, locale,
                                                is_multi_byte_character_support_enabled),
                            ' ');
    }
  }
}

inline bool Printer::print_cell_border_top(std::ostream &stream, TableInternal &table,
                                           const std::pair<size_t, size_t> &index,
                                           const std::pair<size_t, size_t> &dimension,
                                           size_t num_columns) {
  auto &cell = table[index.first][index.second];
  const auto &format = cell.format();
  auto column_width = dimension.second;

  auto corner = *format.corner_top_left_;
  auto corner_color = *format.corner_top_left_color_;
  auto corner_background_color = *format.corner_top_left_background_color_;
  auto corner_style = *format.corner_top_left_style_;
  auto border_top = *format.border_top_;

  if ((corner == "" && border_top == "") || !*format.show_border_top_) {
    return false;
  }

  apply_element_style(stream, corner_color, corner_background_color, corner_style);
  if (*format.show_row_separator_) {
    if (index.first != 0)
      stream << corner;
    else
      stream << " ";
  }
  else
    stream << corner;
  reset_element_style(stream);

  {
    // Apply style once and batch the repeated border character into a
    // single write, instead of once per column-width unit.
    const std::string &repeated_unit =
        (*format.show_row_separator_ && index.first == 0) ? std::string(" ") : border_top;
    std::string line;
    line.reserve(repeated_unit.size() * column_width);
    for (size_t i = 0; i < column_width; ++i)
      line += repeated_unit;

    apply_element_style(stream, *format.border_top_color_, *format.border_top_background_color_,
                        *format.border_top_style_);
    stream << line;
    reset_element_style(stream);
  }

  if (index.second + 1 == num_columns) {
    // Print corner after last column
    corner = *format.corner_top_right_;
    corner_color = *format.corner_top_right_color_;
    corner_background_color = *format.corner_top_right_background_color_;
    corner_style = *format.corner_top_right_style_;

    apply_element_style(stream, corner_color, corner_background_color, corner_style);
    if (*format.show_row_separator_) {
      if (index.first != 0)
        stream << corner;
      else
        stream << " ";
    }
    else
      stream << corner;
    reset_element_style(stream);
  }
  return true;
}

inline bool Printer::print_cell_border_bottom(std::ostream &stream, TableInternal &table,
                                              const std::pair<size_t, size_t> &index,
                                              const std::pair<size_t, size_t> &dimension,
                                              size_t num_columns) {
  auto &cell = table[index.first][index.second];
  const auto &format = cell.format();
  auto column_width = dimension.second;

  auto corner = *format.corner_bottom_left_;
  auto corner_color = *format.corner_bottom_left_color_;
  auto corner_background_color = *format.corner_bottom_left_background_color_;
  auto corner_style = *format.corner_bottom_left_style_;
  auto border_bottom = *format.border_bottom_;

  if ((corner == "" && border_bottom == "") || !*format.show_border_bottom_) {
    return false;
  }

  apply_element_style(stream, corner_color, corner_background_color, corner_style);
  stream << corner;
  reset_element_style(stream);

  {
    // Apply style once and batch the repeated border character into a
    // single write, instead of once per column-width unit.
    std::string line;
    line.reserve(border_bottom.size() * column_width);
    for (size_t i = 0; i < column_width; ++i)
      line += border_bottom;

    apply_element_style(stream, *format.border_bottom_color_,
                        *format.border_bottom_background_color_, *format.border_bottom_style_);
    stream << line;
    reset_element_style(stream);
  }

  if (index.second + 1 == num_columns) {
    // Print corner after last column
    corner = *format.corner_bottom_right_;
    corner_color = *format.corner_bottom_right_color_;
    corner_background_color = *format.corner_bottom_right_background_color_;
    corner_style = *format.corner_bottom_right_style_;

    apply_element_style(stream, corner_color, corner_background_color, corner_style);
    stream << corner;
    reset_element_style(stream);
  }
  return true;
}

} // namespace tabulate
