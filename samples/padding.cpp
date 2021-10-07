#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

int main() {
  Table no_padding;
  no_padding.format().font_style({FontStyle::bold}).padding(0);
  no_padding.add_row(Row_t{"This cell has no padding"});
  std::cout << "Table with no padding:\n" << no_padding << std::endl;

  Table padding_top_bottom;
  padding_top_bottom.format().font_style({FontStyle::bold}).padding(0);
  padding_top_bottom.add_row(
      Row_t{"This cell has padding top = 1", "This cell has padding bottom = 1"});
  padding_top_bottom[0][0].format().padding_top(1);
  padding_top_bottom[0][1].format().padding_bottom(1);

  std::cout << "\nTable with top/bottom padding:\n" << padding_top_bottom << std::endl;
}