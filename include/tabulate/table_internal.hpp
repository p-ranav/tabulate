#include <iostream>
#include <string>
#include <tabulate/font_style.hpp>
#include <tabulate/row.hpp>
#include <tabulate/termcolor.hpp>
#include <vector>

namespace tabulate {

Format &Cell::format() {
  if (!format_.has_value()) { // no cell format
    std::shared_ptr<Row> parent = parent_.lock();
    format_ = parent->format(); // Use parent row format
  }
  return format_.value();
}

class TableInternal : public std::enable_shared_from_this<TableInternal> {
public:
  static std::shared_ptr<TableInternal> create() {
    return std::shared_ptr<TableInternal>(new TableInternal());
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

  Format &format() { return format_; }

  void print(std::ostream &stream) const {
    for (auto &row : rows_)
      for (auto &cell : row->cells())
        if (cell->format().width_.has_value())
          std::cout << cell->format().width_.value() << std::endl;
        else
          std::cout << 0 << std::endl;
  }

private:
  TableInternal() {}

  // Make operator= and C(const C&) private unimplemented
  // so the used cant do bad things like C c( * c_ptr );
  TableInternal &operator=(const TableInternal &);
  TableInternal(const TableInternal &);

  std::vector<std::shared_ptr<Row>> rows_;
  Format format_;
};

Format &Row::format() {
  if (!format_.has_value()) { // no row format
    std::shared_ptr<TableInternal> parent = parent_.lock();
    format_ = parent->format(); // Use parent table format
  }
  return format_.value();
}

} // namespace tabulate