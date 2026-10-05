#include <iostream>
#include <tabulate/table.hpp>

using namespace tabulate;

// Demonstrates Table::paginate(): splitting a large table into
// consistently-formatted pages. Column widths are computed once from the
// whole table, so every page lines up identically even though the content on
// each page differs.
int main() {
  Table table;
  table.add_row({"ID", "Name", "Status"});
  for (int i = 1; i <= 1000; ++i)
    table.add_row({std::to_string(i), "Item " + std::to_string(i), i % 7 == 0 ? "FAILED" : "OK"});

  std::vector<Table> pages = table.paginate(/*rows_per_page=*/250);

  std::cout << "Paginated " << table.size() - 1 << " data rows into " << pages.size()
            << " pages of up to 250 rows each.\n\n";

  // Print just the first and last page here to keep sample output short.
  std::cout << "Page 1 of " << pages.size() << '\n';
  std::cout << pages.front() << "\n\n";

  std::cout << "Page " << pages.size() << " of " << pages.size() << '\n';
  std::cout << pages.back() << '\n';
}
