#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

// Demonstrates the fix for https://github.com/p-ranav/tabulate/issues/111:
// clean Unicode box-drawing borders/corners instead of the default ASCII
// "+"/"-"/"|", without having to manually set each row's corners/borders.
int main() {
  Table outer_box_only;
  outer_box_only.add_row(Row_t{"Quantity", "Value"});
  outer_box_only.add_row(Row_t{"A", "1"});
  outer_box_only.add_row(Row_t{"B", "2"});
  outer_box_only.use_unicode_borders();
  std::cout << "Outer box only:\n" << outer_box_only << "\n\n";

  Table with_row_separators;
  with_row_separators.add_row(Row_t{"Quantity", "Value"});
  with_row_separators.add_row(Row_t{"A", "1"});
  with_row_separators.add_row(Row_t{"B", "2"});
  with_row_separators.use_unicode_borders(true);
  std::cout << "With row separators:\n" << with_row_separators << "\n\n";

  Table heavy;
  heavy.add_row(Row_t{"col1", "col2"});
  heavy.add_row(Row_t{"val1", "val2"});
  heavy.use_unicode_borders(true, BorderStyle::Heavy);
  std::cout << "Heavy style:\n" << heavy << "\n\n";

  Table double_line;
  double_line.add_row(Row_t{"col1", "col2"});
  double_line.add_row(Row_t{"val1", "val2"});
  double_line.use_unicode_borders(true, BorderStyle::Double);
  std::cout << "Double style:\n" << double_line << std::endl;
}
