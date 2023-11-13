#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

int main() {
  Table table;
  table.format().trim_mode(Format::TrimMode::kNone);
  table.add_row(Row_t{"This is\na cell that\nspans\nmultiple lines"});
  table.add_row(Row_t{"{\n  foo\n  bar\n}"});
  std::cout << table << std::endl;
}