#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

int main() {
  Table table;

  table.add_row(Row_t{"Company", "Contact", "Country"});
  table.add_row(Row_t{"Alfreds Futterkiste", "Maria Anders", "Germany"});
  table.add_row(Row_t{"Centro comercial Moctezuma", "Francisco Chang", "Mexico"});
  table.add_row(Row_t{"Ernst Handel", "Roland Mendel", "Austria"});
  table.add_row(Row_t{"Island Trading", "Helen Bennett", "UK"});
  table.add_row(Row_t{"Laughing Bacchus Winecellars", "Yoshi Tannamuri", "Canada"});
  table.add_row(Row_t{"Magazzini Alimentari Riuniti", "Giovanni Rovelli", "Italy"});

  // Set width of cells in each column
  table.column(0).format().width(40);
  table.column(1).format().width(30);
  table.column(2).format().width(30);

  // Iterate over cells in the first row
  for (auto &cell : table[0]) {
    cell.format().font_style({FontStyle::underline}).font_align(FontAlign::center);
  }

  // Iterator over cells in the second column
  for (auto &cell : table.column(0)) {
    if (cell.get_text() != "Company") {
      cell.format().font_align(FontAlign::right);
    }
  }

  // Iterate over rows in the table
  size_t index = 0;
  for (auto &row : table) {
    row.format().font_style({FontStyle::bold});

    // Set blue background color for alternate rows
    if (index > 0 && index % 2 == 0) {
      for (auto &cell : row) {
        cell.format().font_background_color(Color::blue);
      }
    }
    index += 1;
  }

  std::cout << table << std::endl;
}