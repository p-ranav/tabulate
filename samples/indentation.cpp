#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

// Demonstrates the fix for https://github.com/p-ranav/tabulate/issues/96:
// Format::indent(n) prefixes every rendered line with n spaces, shifting the
// whole table to the right in the terminal.
int main() {
  Table table;
  table.add_row(Row_t{"Command", "Description"});
  table.add_row(Row_t{"git status", "List all new or modified files"});
  table.format().indent(4);

  std::cout << table << std::endl;
}
