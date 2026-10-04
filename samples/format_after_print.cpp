#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

// Regression sample for https://github.com/p-ranav/tabulate/issues/80
// Changing table.format() after the table has already been printed once used
// to have no effect, because Row/Cell cached their fully-merged format on the
// first print and never re-synced with later changes to the table's format.
int main() {
  Table table;
  table.add_row(Row_t{"hello"});
  table.add_row(Row_t{"world"});

  std::cout << table << std::endl;

  table.format().hide_border_bottom();
  std::cout << table << std::endl;

  table.format().width(50);
  std::cout << table << std::endl;
}
