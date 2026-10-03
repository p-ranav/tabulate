#include <clocale>
#include <cstdio>
#include <cstdlib>
#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

static void report(const char *stage) {
  std::printf("%-7s LC_CTYPE=%-12s MB_CUR_MAX=%d\n", stage, std::setlocale(LC_CTYPE, nullptr),
              static_cast<int>(MB_CUR_MAX));
}

int main() {
  std::setlocale(LC_ALL, "");
  report("before");

  Table table;
  table.add_row(Row_t{"one", "two"});
  std::cout << table << std::endl;

  report("after");
}
