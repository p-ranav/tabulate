#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include <cstdlib>
#include <iostream>
#include <string>

namespace {
int failures = 0;
int checks = 0;

void expect(const char *name, tabulate::Table &table, const std::string &expected) {
  ++checks;
  const std::string actual = table.str();
  if (actual != expected) {
    ++failures;
    std::cerr << name << "\nExpected:\n" << expected << "\nActual:\n" << actual << '\n';
  }
}
} // namespace

int main() {
  tabulate::Table single;
  single.add_row({""});
  expect("one empty cell", single, "+--+\n|  |\n+--+");

  tabulate::Table between;
  between.add_row({"header"});
  between.add_row({""});
  between.add_row({"end"});
  expect("empty row between populated rows", between,
         "+--------+\n| header |\n+--------+\n|        |\n+--------+\n| end    |\n+--------+");

  tabulate::Table multiple;
  multiple.add_row({"left", "right"});
  multiple.add_row({"", ""});
  multiple.add_row({"end", "done"});
  expect("multiple empty columns", multiple,
         "+------+-------+\n| left | right |\n+------+-------+\n|      |       |\n"
         "+------+-------+\n| end  | done  |\n+------+-------+");

  tabulate::Table consecutive;
  consecutive.add_row({""});
  consecutive.add_row({""});
  expect("consecutive empty rows", consecutive, "+--+\n|  |\n+--+\n|  |\n+--+");

  tabulate::Table padded;
  padded.add_row({""});
  padded[0].format().padding_top(1).padding_bottom(2);
  expect("padding surrounds an empty content line", padded,
         "+--+\n|  |\n|  |\n|  |\n|  |\n+--+");

  tabulate::Table zero_padding;
  zero_padding.add_row({""});
  zero_padding[0].format().padding(0);
  expect("zero-width empty cell", zero_padding, "++\n||\n++");

  tabulate::Table height;
  height.add_row({""});
  height[0].format().height(3);
  expect("explicit height is preserved", height, "+--+\n|  |\n|  |\n|  |\n+--+");

  tabulate::Table mixed;
  mixed.add_row({"", "value"});
  expect("mixed empty and nonempty cells", mixed, "+--+-------+\n|  | value |\n+--+-------+");

  tabulate::Table nonempty;
  nonempty.add_row({"value"});
  expect("nonempty control", nonempty, "+-------+\n| value |\n+-------+");

  tabulate::Table multiline;
  multiline.add_row({"a\nb"});
  expect("multiline control", multiline, "+---+\n| a |\n| b |\n+---+");

  tabulate::Table trailing;
  trailing.add_row({"a\n"});
  // Preserve the existing width calculation for trailing/newline-only text.
  expect("trailing newline control", trailing, "+----+\n| a  |\n+----+");

  tabulate::Table newline;
  newline.add_row({"\n"});
  expect("newline-only control", newline, "+---+\n|   |\n+---+");

  std::cout << checks - failures << '/' << checks << " checks passed\n";
  return failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}
