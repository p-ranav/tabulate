#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

// Demonstrates the fix for https://github.com/p-ranav/tabulate/issues/62:
// print the header immediately, then stream each row to stdout as soon as
// it's computed/added, instead of waiting to print the whole table at once.
//
// This only works with fixed column widths: a later, wider row can't
// retroactively widen a column whose border has already been printed.
int main() {
  Table table;
  table.add_row(Row_t{"Step", "Result"});
  table.column(0).format().width(10);
  table.column(1).format().width(12);
  table.print_row(0); // print the header as soon as it's added

  for (int step = 1; step <= 3; ++step) {
    // ... pretend some computation happens here ...
    table.add_row(Row_t{"Step " + std::to_string(step), step % 2 == 0 ? "OK" : "FAILED"});
    table.print_row(table.size() - 1); // stream the new row immediately
  }

  table.print_bottom_border();
}
