#include <tabulate/table_internal.hpp>
#include <variant>

namespace tabulate {

class Table {
public:
  Table() : table_(TableInternal::create()) {}

  Table &add_row(const std::vector<std::variant<std::string, Table>> &cells) {

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

    for (size_t i = 0; i < cells.size(); ++i) {
      auto cell = cells[i];
      if (std::holds_alternative<std::string>(cell)) {
        cell_strings[i] = std::get<std::string>(cell);
      } else {
        auto table = std::get<Table>(cell);
        std::stringstream stream;
        table.print(stream);
        cell_strings[i] = stream.str();
      }
    }

    table_->add_row(cell_strings);
    rows_ += 1;
    return *this;
  }

  Row &operator[](size_t index) { return row(index); }

  Row &row(size_t index) { return (*table_)[index]; }

  Column column(size_t index) { return table_->column(index); }

  Format &format() { return table_->format(); }

  void print(std::ostream &stream) { table_->print(stream); }

  std::string str() {
    std::stringstream stream;
    print(stream);
    return stream.str();
  }

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

  auto begin() { return RowIterator(table_->rows_.begin()); }
  auto end() { return RowIterator(table_->rows_.end()); }

private:
  friend std::ostream &operator<<(std::ostream &stream, const Table &table);
  size_t rows_{0};
  size_t cols_{0};
  std::shared_ptr<TableInternal> table_;
};

std::ostream &operator<<(std::ostream &stream, const Table &table) {
  const_cast<Table &>(table).print(stream);
  return stream;
}

} // namespace tabulate
