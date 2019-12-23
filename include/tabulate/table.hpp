#include <iostream>
#include <string>
#include <tabulate/font_style.hpp>
#include <tabulate/row.hpp>
#include <tabulate/termcolor.hpp>
#include <vector>

namespace tabulate {

Format &Cell::format() {
  if (!format_.has_value()) {         // no cell format
    std::shared_ptr<Row> parent = parent_.lock();
    format_ = parent->format(); // Use parent row format
  }
  return format_.value();
}

class Table {
public:
  void add_row(const std::vector<std::string> &cells) {
    auto row = std::make_shared<Row>(this);
    for (auto &c : cells) {
      auto cell = std::make_shared<Cell>(row);
      cell->set_text(c);
      row->add_cell(cell);
    }
    rows_.push_back(row);
  }

  Format &format() { return format_; }

private:
  friend std::ostream &operator<<(std::ostream &os, const Table &table);

  void print(std::ostream& stream) const {
    for (auto& row : rows_)
      for (auto& cell : row->cells())
        if (cell->format().width_.has_value())
          std::cout << cell->format().width_.value() << std::endl;
  }

  std::vector<std::shared_ptr<Row>> rows_;
  Format format_;
};

Format &Row::format() {
  if (!format_.has_value()) {      // no row format
    format_ = parent_->format(); // Use parent table format
  }
  return format_.value();
}

std::ostream &operator<<(std::ostream &stream, const Table &table) {
  table.print(stream);
  return stream;
}

} // namespace tabulate
