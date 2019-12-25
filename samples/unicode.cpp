#include <tabulate/table.hpp>
using namespace tabulate;

int main() {
  Table table;
  table.add_row({"🔥 "});
  std::cout << table << std::endl;
}