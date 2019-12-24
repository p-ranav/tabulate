#include <tabulate/table_internal.hpp>
#include <variant>

namespace tabulate {

class Table {
public:
  Table() { table_ = TableInternal::create(); }

  void add_row(const std::vector<std::variant<std::string, Table>> &cells) {
    std::vector<std::string> cell_strings;

    for (auto& cell : cells) {
      if (std::holds_alternative<std::string>(cell)) {
        cell_strings.push_back(std::get<std::string>(cell));
      } else {
        auto table = std::get<Table>(cell);
        std::stringstream stream;
        table.print(stream);
        cell_strings.push_back(stream.str());
      }
    }

    table_->add_row(cell_strings);
  }

  Row &operator[](size_t index) { return row(index); }

  Row &row(size_t index) { return (*table_)[index]; }

  Column column(size_t index) { return table_->column(index); }

  Format &format() { return table_->format(); }

  void print(std::ostream &stream) { table_->print(stream); }

private:
  friend std::ostream& operator<<(std::ostream& stream, const Table& table);

  std::shared_ptr<TableInternal> table_;
};

std::ostream& operator<<(std::ostream& stream, const Table& table) {
  const_cast<Table&>(table).print(stream);
  return stream;
}

} // namespace tabulate
