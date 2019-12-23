#include <tabulate/table_internal.hpp>

namespace tabulate {

class Table {
public:
  Table() {
    table_ = TableInternal::create();
  }

  void add_row(const std::vector<std::string> &cells) {
    table_->add_row(cells);
  }

  Row& operator[](size_t index) {
    return (*table_)[index];
  }

private:
  friend std::ostream &operator<<(std::ostream &os, const Table &table);
  std::shared_ptr<TableInternal> table_;
};

std::ostream &operator<<(std::ostream &os, const Table &table) {
  table.table_->print(os);
  return os;
}

} // namespace tabulate
