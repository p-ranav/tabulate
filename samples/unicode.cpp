#include <tabulate/table.hpp>
using namespace tabulate;

int main() {
  Table table;
  table.add_row({"🔥 🔥  ABC DEF 🔥 🔥"});
  std::cout << table << std::endl;
}