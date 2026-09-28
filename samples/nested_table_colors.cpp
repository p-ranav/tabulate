#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

int main() {
  Table inner;
  inner.add_row(Row_t{"A"});
  inner[0][0].format().font_color(Color::green);

  Table outer;
  outer.add_row(Row_t{inner});

  std::cout << "on its own:\n" << inner << "\n\n";
  std::cout << "nested in another table:\n" << outer << std::endl;
}
