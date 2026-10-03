#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

int main() {
  Table employees;
  employees.add_row(Row_t{"Name", "Age", "Country", "Notes"});
  employees.add_row(Row_t{"Alice", "30", "USA", ""});
  employees.add_row(Row_t{"Bob", "28", "UK", ""});
  employees.add_row(Row_t{"Charlie", "35", "Canada", ""});

  std::cout << "Before removing empty column:\n" << employees << "\n\n";

  // The "Notes" column (index 3) is empty for every row, so drop it.
  employees.erase_column(3);

  std::cout << "After removing empty column:\n" << employees << std::endl;
}
