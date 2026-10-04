#include <tabulate/table.hpp>
using namespace tabulate;
using Row_t = Table::Row_t;

// Demonstrates the fix for https://github.com/p-ranav/tabulate/issues/86
// By default, each line of a multi-line cell has leading/trailing whitespace
// trimmed, which destroys indentation in pretty-printed markup like JSON/XML.
// Format::trim_mode(TrimMode::kNone) preserves it.
int main() {
  std::string json = "{\n"
                      "  \"name\": \"tabulate\",\n"
                      "  \"tags\": [\n"
                      "    \"cpp\",\n"
                      "    \"table\"\n"
                      "  ]\n"
                      "}";

  Table default_trim;
  default_trim.add_row(Row_t{"Default trim (kBoth)", json});
  std::cout << default_trim << "\n\n";

  Table no_trim;
  no_trim.add_row(Row_t{"trim_mode(kNone)", json});
  no_trim[0][1].format().trim_mode(Format::TrimMode::kNone);
  std::cout << no_trim << std::endl;
}
