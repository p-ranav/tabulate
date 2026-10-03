#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

int main() {
  // Cell contents that already carry their own ANSI escape sequences, e.g.
  // strings colored by the application or produced by a nested table
  const std::string red = "\x1b[31mred\x1b[0m";
  const std::string naive = "\x1b[1mnaïve\x1b[0m";

  Table table;
  table.add_row(Row_t{"content", "visible width"});
  table.add_row(Row_t{red, "3"});
  table.add_row(Row_t{naive, "5"});
  table.format().multi_byte_characters(true);

  // The escape sequences occupy no columns, so the first column is three
  // characters wide and the borders line up with what the terminal shows
  std::cout << table << std::endl;
}
