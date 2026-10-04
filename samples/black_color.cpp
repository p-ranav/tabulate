#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

// Demonstrates the fix for https://github.com/p-ranav/tabulate/issues/94:
// Color::black did not exist, so writing black text on a colorful background
// had no direct, discoverable way to do it.
int main() {
  Table table;
  table.add_row(Row_t{"Black text on a yellow background"});
  table[0][0].format().font_color(Color::black).font_background_color(Color::yellow);

  std::cout << table << std::endl;
}
