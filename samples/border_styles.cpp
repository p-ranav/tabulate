#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

// Demonstrates the fix for https://github.com/p-ranav/tabulate/issues/30:
// font_style can now be applied to borders and corners, not just cell text.
int main() {
  Table table;
  table.add_row(Row_t{"Bold border", "Bold corners"});

  table.format()
      .border_style({FontStyle::bold})
      .corner_style({FontStyle::bold})
      .border_color(Color::cyan)
      .corner_color(Color::cyan);

  // A single side can still be styled independently.
  table[0][0].format().border_top_style({FontStyle::bold, FontStyle::underline});

  std::cout << table << std::endl;
}
