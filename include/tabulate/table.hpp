
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
#include <tabulate/table_internal.hpp>

#if __cplusplus >= 201703L
#include <string_view>
#include <variant>
#else
#include <tabulate/string_view_lite.hpp>
#include <tabulate/variant_lite.hpp>
#endif

#include <utility>

namespace tabulate {

#if __cplusplus >= 201703L
using std::get_if;
using std::holds_alternative;
using std::string_view;
using std::variant;
using std::visit;
#else
using nonstd::get_if;
using nonstd::holds_alternative;
using nonstd::string_view;
using nonstd::variant;
using nonstd::visit;
#endif

// Unicode box-drawing glyph sets, for use with Table::use_unicode_borders().
enum class BorderStyle { Light, Heavy, Double };

class Table {
public:
  Table() : table_(TableInternal::create()) {}

  using Row_t = std::vector<variant<std::string, const char *, string_view, Table>>;

  Table &add_row(const Row_t &cells) {

    if (rows_ == 0) {
      // This is the first row added
      // cells.size() is the number of columns
      cols_ = cells.size();
    }

    std::vector<std::string> cell_strings;
    if (cells.size() < cols_) {
      cell_strings.resize(cols_);
      std::fill(cell_strings.begin(), cell_strings.end(), "");
    } else {
      cell_strings.resize(cells.size());
      std::fill(cell_strings.begin(), cell_strings.end(), "");
    }

    std::vector<size_t> nested_table_indices;
    for (size_t i = 0; i < cells.size(); ++i) {
      auto cell = cells[i];
      if (holds_alternative<std::string>(cell)) {
        cell_strings[i] = *get_if<std::string>(&cell);
      } else if (holds_alternative<const char *>(cell)) {
        cell_strings[i] = *get_if<const char *>(&cell);
      } else if (holds_alternative<string_view>(cell)) {
        cell_strings[i] = std::string{*get_if<string_view>(&cell)};
      } else {
        auto table = *get_if<Table>(&cell);
        std::stringstream stream;
        // termcolor emits nothing on a stream that is not a terminal, which
        // would strip every color the nested table asked for. The destination
        // is not known until the outer table is printed, so follow std::cout.
        if (termcolor::_internal::is_colorized(std::cout))
          stream << termcolor::colorize;
        table.print(stream);
        cell_strings[i] = stream.str();
        nested_table_indices.push_back(i);
      }
    }

    table_->add_row(cell_strings);
    // A nested table's text is pre-formatted ASCII art: trimming would corrupt its layout.
    for (auto i : nested_table_indices)
      row(rows_)[i].format().trim_mode(Format::TrimMode::kNone);
    rows_ += 1;
    return *this;
  }

  Row &operator[](size_t index) { return row(index); }

  Row &row(size_t index) { return (*table_)[index]; }

  Column column(size_t index) { return table_->column(index); }

  Table &erase_column(size_t index) {
    table_->erase_column(index);
    if (cols_ > 0)
      cols_ -= 1;
    return *this;
  }

  // Configures clean Unicode box-drawing borders/corners instead of the
  // default ASCII "+"/"-"/"|", in one of the common box-drawing weights
  // (BorderStyle::Light "┌─┬─┐", ::Heavy "┏━┳━┓", or ::Double "╔═╦═╗").
  // With show_row_separators = false (the default), only the outer box is
  // drawn; with true, every row boundary gets the matching T-junction/cross
  // characters too.
  Table &use_unicode_borders(bool show_row_separators = false, BorderStyle style = BorderStyle::Light) {
    struct Glyphs {
      const char *horizontal;
      const char *vertical;
      const char *top_left, *top_mid, *top_right;
      const char *mid_left, *mid_mid, *mid_right;
      const char *bottom_left, *bottom_mid, *bottom_right;
    };

    Glyphs glyphs;
    switch (style) {
    case BorderStyle::Heavy:
      glyphs = {"\u2501", "\u2503", "\u250f", "\u2533", "\u2513",
                "\u2523", "\u254b", "\u252b", "\u2517", "\u253b", "\u251b"};
      break;
    case BorderStyle::Double:
      glyphs = {"\u2550", "\u2551", "\u2554", "\u2566", "\u2557",
                "\u2560", "\u256c", "\u2563", "\u255a", "\u2569", "\u255d"};
      break;
    case BorderStyle::Light:
    default:
      glyphs = {"\u2500", "\u2502", "\u250c", "\u252c", "\u2510",
                "\u251c", "\u253c", "\u2524", "\u2514", "\u2534", "\u2518"};
      break;
    }

    format()
        .border_left(glyphs.vertical)
        .border_right(glyphs.vertical)
        .border_top(glyphs.horizontal)
        .border_bottom(glyphs.horizontal);

    size_t num_rows = size();
    for (size_t i = 0; i < num_rows; ++i) {
      Row &r = row(i);
      size_t num_cols = r.size();
      bool is_first_row = (i == 0);
      bool is_last_row = (i + 1 == num_rows);
      for (size_t j = 0; j < num_cols; ++j) {
        bool is_first_col = (j == 0);
        bool is_last_col = (j + 1 == num_cols);
        Cell &cell = r[j];

        if (is_first_row) {
          cell.format().corner_top_left(is_first_col ? glyphs.top_left : glyphs.top_mid);
          if (is_last_col)
            cell.format().corner_top_right(glyphs.top_right);
        } else if (show_row_separators) {
          cell.format().corner_top_left(is_first_col ? glyphs.mid_left : glyphs.mid_mid);
          if (is_last_col)
            cell.format().corner_top_right(glyphs.mid_right);
        } else {
          cell.format().hide_border_top();
        }

        if (is_last_row) {
          cell.format().corner_bottom_left(is_first_col ? glyphs.bottom_left : glyphs.bottom_mid);
          if (is_last_col)
            cell.format().corner_bottom_right(glyphs.bottom_right);
        }
      }
    }
    return *this;
  }

  Format &format() { return table_->format(); }

  void print(std::ostream &stream) { table_->print(stream); }

  // Prints just one row (see TableInternal::print_row() for the column-width
  // caveat around streaming rows as they're added).
  void print_row(size_t index, std::ostream &stream = std::cout) { table_->print_row(stream, index); }

  // Prints the table's closing bottom border. Pair with print_row() once
  // the last row has been added and printed.
  void print_bottom_border(std::ostream &stream = std::cout) { table_->print_bottom_border(stream); }

  std::string str() {
    std::stringstream stream;
    print(stream);
    return stream.str();
  }

  size_t size() const { return table_->size(); }

  // {number of rows, number of columns} actually added to the table. Not to
  // be confused with shape(), which measures the *rendered* table instead.
  std::pair<size_t, size_t> dimensions() const { return {size(), cols_}; }

  std::pair<size_t, size_t> shape() { return table_->shape(); }

  class RowIterator {
  public:
    explicit RowIterator(std::vector<std::shared_ptr<Row>>::iterator ptr) : ptr(ptr) {}

    RowIterator operator++() {
      ++ptr;
      return *this;
    }
    bool operator!=(const RowIterator &other) const { return ptr != other.ptr; }
    Row &operator*() { return **ptr; }

  private:
    std::vector<std::shared_ptr<Row>>::iterator ptr;
  };

  auto begin() -> RowIterator { return RowIterator(table_->rows_.begin()); }
  auto end() -> RowIterator { return RowIterator(table_->rows_.end()); }

private:
  friend class MarkdownExporter;
  friend class LatexExporter;
  friend class AsciiDocExporter;

  friend std::ostream &operator<<(std::ostream &stream, const Table &table);
  size_t rows_{0};
  size_t cols_{0};
  std::shared_ptr<TableInternal> table_;
};

inline std::ostream &operator<<(std::ostream &stream, const Table &table) {
  const_cast<Table &>(table).print(stream);
  return stream;
}

class RowStream {
public:
  operator const Table::Row_t &() const { return row_; }

  template <typename T, typename = typename std::enable_if<
                            !std::is_convertible<T, Table::Row_t::value_type>::value>::type>
  RowStream &operator<<(const T &obj) {
    oss_ << obj;
    std::string cell{oss_.str()};
    oss_.str("");
    if (!cell.empty()) {
      row_.push_back(cell);
    }
    return *this;
  }

  RowStream &operator<<(const Table::Row_t::value_type &cell) {
    row_.push_back(cell);
    return *this;
  }

  RowStream &copyfmt(const RowStream &other) {
    oss_.copyfmt(other.oss_);
    return *this;
  }

  RowStream &copyfmt(const std::ios &other) {
    oss_.copyfmt(other);
    return *this;
  }

  std::ostringstream::char_type fill() const { return oss_.fill(); }
  std::ostringstream::char_type fill(std::ostringstream::char_type ch) { return oss_.fill(ch); }

  std::ios_base::iostate exceptions() const { return oss_.exceptions(); }
  void exceptions(std::ios_base::iostate except) { oss_.exceptions(except); }

  std::locale imbue(const std::locale &loc) { return oss_.imbue(loc); }
  std::locale getloc() const { return oss_.getloc(); }

  char narrow(std::ostringstream::char_type c, char dfault) const { return oss_.narrow(c, dfault); }
  std::ostringstream::char_type widen(char c) const { return oss_.widen(c); }

  std::ios::fmtflags flags() const { return oss_.flags(); }
  std::ios::fmtflags flags(std::ios::fmtflags flags) { return oss_.flags(flags); }

  std::ios::fmtflags setf(std::ios::fmtflags flags) { return oss_.setf(flags); }
  std::ios::fmtflags setf(std::ios::fmtflags flags, std::ios::fmtflags mask) {
    return oss_.setf(flags, mask);
  }

  void unsetf(std::ios::fmtflags flags) { oss_.unsetf(flags); }

  std::streamsize precision() const { return oss_.precision(); }
  std::streamsize precision(std::streamsize new_precision) { return oss_.precision(new_precision); }

  std::streamsize width() const { return oss_.width(); }
  std::streamsize width(std::streamsize new_width) { return oss_.width(new_width); }

private:
  Table::Row_t row_;
  std::ostringstream oss_;
};

} // namespace tabulate
