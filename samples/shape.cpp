#include <tabulate/table.hpp>
using namespace tabulate;

void print_shape(Table &table) {
  auto shape = table.shape();
  std::cout << "Shape: (" << shape.first << ", " << shape.second << ")" << std::endl;
}

int main() {
  Table table;
  table.add_row({"Command", "Description"});
  table.add_row({"git status", "List all new or modified files"});
  table.add_row({"git diff", "Show file differences that haven't been staged"});
  std::cout << table << std::endl;
  print_shape(table);
}
