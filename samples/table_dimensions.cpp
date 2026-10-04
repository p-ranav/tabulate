#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

// Demonstrates the fix for https://github.com/p-ranav/tabulate/issues/107:
// shape() measures the *rendered* table (character width x line count), not
// the logical row/column count. dimensions() gives the latter.
int main() {
  Table table;
  table.add_row(Row_t{"cell"});

  auto rendered = table.shape();
  std::cout << "shape() [rendered width x height]: (" << rendered.first << ", " << rendered.second
            << ")\n";

  auto logical = table.dimensions();
  std::cout << "dimensions() [rows x columns]: (" << logical.first << ", " << logical.second << ")"
            << std::endl;
}
