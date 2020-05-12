#include <tabulate/table.hpp>
using namespace tabulate;

#if __cplusplus >= 201703L
#include <variant>
using std::variant;
#else
#include <tabulate/variant_lite.hpp>
using nonstd::variant;
#endif
using Row_t = std::vector<variant<std::string, const char *, Table>>;

void print_shape(Table &table) {
  auto shape = table.shape();
  std::cout << "Shape: (" << shape.first << ", " << shape.second << ")" << std::endl;
}

int main() {
  Table table;
  table.add_row(Row_t{"Command", "Description"});
  table.add_row(Row_t{"git status", "List all new or modified files"});
  table.add_row(Row_t{"git diff", "Show file differences that haven't been staged"});
  std::cout << table << std::endl;
  print_shape(table);
}
