#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

int main() {
  Table table;

  auto lorem = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor "
               "incididunt ut labore et dolore magna aliqua.";

  table.add_row(Row_t{"Regular cells", Merge{2}});
  table.add_row(Row_t{lorem, lorem, lorem});
  table.add_row(Row_t{"Merge two left cells", Merge{2}});
  table.add_row(Row_t{lorem, Merge{}, lorem});
  table.add_row(Row_t{"Merge two2 right cells", Merge{2}});
  table.add_row(Row_t{lorem, lorem, Merge{}});
  table.add_row(Row_t{"Merge all cells, different alignment", Merge{2}});
  table.add_row(Row_t{lorem, Merge{}, Merge{}});
  table.add_row(Row_t{lorem, Merge{2}});
  table.add_row(Row_t{lorem, Merge{2}});

  table[0][0].format().width(20);
  table[0][1].format().width(50);
  table[0][2].format().width(30);

  table[0][0].format().font_align(FontAlign::center);
  table[2][0].format().font_align(FontAlign::center);
  table[4][0].format().font_align(FontAlign::center);
  table[5][1].format().font_align(FontAlign::right);
  table[6][0].format().font_align(FontAlign::center);
  table[8][0].format().font_align(FontAlign::center);
  table[9][0].format().font_align(FontAlign::right);

  std::cout << table << std::endl;
}
