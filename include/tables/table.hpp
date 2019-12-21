#include <iostream>
#include <vector>
#include <string>
#include <tables/row.hpp>
#include <tables/termcolor.hpp>

namespace tables {

class Table {
public:

  void add_row(const std::vector<std::string>& cells) {
    Row row;
    for (auto& cell : cells) {
      row.add_cell(Cell(cell));
    }
    rows_.push_back(row);
  }

  Row& operator[](size_t index) {
    return rows_[index];
  }

  Format& format() {
    return format_;
  }

  void print(std::ostream& stream = std::cout) {
    for (size_t i = 0; i < rows_.size(); ++i) {
      auto& row = rows_[i];
      auto row_height = get_row_height(i);
      const auto& cells = row.cells();
      // Header row
      for (size_t j = 0; j < cells.size(); ++j) {
	print_cell_header(stream, i, j);
      }
      stream << "\n";
      // Padding top
      for (size_t k = 0; k < format_.padding_top_; ++k) {
	print_padding_row(stream, i);
	stream << "\n";
      }
      // Row contents
      for (size_t k = 0; k < row_height; ++k) {
	std::vector<std::string> sub_row_contents;
	std::vector<size_t> column_widths;
	for (size_t l = 0; l < cells.size(); ++l) {
	  auto column_width = get_column_width(l);
	  auto cell_contents = cells[l].data();
	  auto pos = k * column_width;
	  auto size = cell_contents.size();	  
	  if (pos < size) {
	    auto remaining = (size - pos);
	    sub_row_contents.push_back(cell_contents.substr(pos, std::min(remaining, column_width)));
	  } else {
	    sub_row_contents.push_back("");
	  }
	  column_widths.push_back(column_width);
	}
	print_content_row(stream, sub_row_contents, column_widths);
      }
      
      // Padding bottom
      for (size_t k = 0; k < format_.padding_bottom_; ++k) {
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
    stream << "\n";
  }

private:

  size_t get_column_width(size_t index) {
    size_t result{0};
    for (auto& row : rows_) {
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

  size_t get_row_height(size_t index) {
    size_t result{1};
    if (index < rows_.size()) {
      result = std::max(result, rows_[index].height());
    }
    return result;
  }

  void print_padding_row(std::ostream& stream, size_t row_index) {
    for (size_t col_index = 0; col_index < rows_[row_index].size(); ++col_index) {
      auto width = get_column_width(col_index);
      auto height = get_row_height(row_index);
      auto cell = rows_[row_index].get_cell(col_index);
      
      // add padding to width
      width += format_.padding_left_;
      width += format_.padding_right_;
      
      stream << format_.border_left_;
      
      size_t i = 0;
      while(i < width) {
	stream << " ";
	++i;
      }
    }
    stream << format_.border_right_;
  }

  void print_content_row(std::ostream& stream, std::vector<std::string> row_contents, std::vector<size_t> column_widths) {
    for (size_t i = 0; i < row_contents.size(); ++i) {
      auto cell_content = row_contents[i];
      stream << format_.border_left_;
      for (size_t j = 0; j < format_.padding_left_; ++j) {
	stream << " ";     
      }
      stream << cell_content;
      auto content_width = cell_content.size();
      auto column_width = column_widths[i];
      if (content_width < column_width) {
	for (size_t j = 0; j < (column_width - content_width); ++j) {
	  stream << " ";
	}
      }
      for (size_t j = 0; j < format_.padding_right_; ++j) {
	stream << " ";
      }
    }
    stream << format_.border_right_;
    std::cout << "\n";
  }

  void print_cell_header(std::ostream& stream, size_t row_index, size_t col_index) {
    auto width = get_column_width(col_index);
    auto height = get_row_height(row_index);
    auto cell = rows_[row_index].get_cell(col_index);
    std::string cell_contents{""};
    if (cell.has_value()) {
      cell_contents = cell.value().data();
    }

    // Print first row of cell
    
    // add padding to width
    width += format_.padding_left_;
    width += format_.padding_right_;

    if (col_index == 0)
      stream << format_.corners_;

    size_t i = 0;
    while(i < width) {
      stream << format_.border_top_;
      ++i;
    }

    stream << format_.corners_;
  }

  void print_cell_footer(std::ostream& stream, size_t row_index, size_t col_index) {
    auto width = get_column_width(col_index);
    auto height = get_row_height(row_index);
    auto cell = rows_[row_index].get_cell(col_index);
    std::string cell_contents{""};
    if (cell.has_value()) {
      cell_contents = cell.value().data();
    }

    // Print first row of cell
    
    // add padding to width
    width += format_.padding_left_;
    width += format_.padding_right_;

    if (col_index == 0)
      stream << format_.corners_;

    size_t i = 0;
    while(i < width) {
      stream << format_.border_bottom_;
      ++i;
    }

    stream << format_.corners_;
  }  
  
  std::vector<Row> rows_;
  Format format_;
};

}
