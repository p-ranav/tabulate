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

int main() {
  Table table;

  table.add_row(
      Row_t{"This paragraph contains a veryveryveryveryveryverylong word. The long word will "
            "break and word wrap to the next line.",
            "This paragraph \nhas embedded '\\n' \ncharacters and\n will break\n exactly "
            "where\n you want it\n to\n break."});

  table[0][0].format().width(20);
  table[0][1].format().width(50);

  std::cout << table << std::endl;
}