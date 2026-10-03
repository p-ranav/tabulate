#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

// Regression sample for https://github.com/p-ranav/tabulate/issues/56
// A nested table containing a multi-line cell with a hidden left border used
// to render with misaligned/missing vertical borders in the outer table.
int main() {
  Table table;
  table.add_row(Row_t{"key", "value1\nvalue2"});
  table[0][0].format().hide_border_left();

  Table table2;
  table2.add_row(Row_t{table});

  std::cout << "first table :\n" << table << std::endl;
  std::cout << "second table :\n" << table2 << std::endl;
}
