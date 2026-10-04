#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include "test_utils.hpp"

int main() {
  tabulate_test::Harness t;

  tabulate::Table single;
  single.add_row({""});
  t.expect("one empty cell", single, "+--+\n|  |\n+--+");

  tabulate::Table between;
  between.add_row({"header"});
  between.add_row({""});
  between.add_row({"end"});
  t.expect("empty row between populated rows", between,
           "+--------+\n| header |\n+--------+\n|        |\n+--------+\n| end    |\n+--------+");

  tabulate::Table multiple;
  multiple.add_row({"left", "right"});
  multiple.add_row({"", ""});
  multiple.add_row({"end", "done"});
  t.expect("multiple empty columns", multiple,
           "+------+-------+\n| left | right |\n+------+-------+\n|      |       |\n"
           "+------+-------+\n| end  | done  |\n+------+-------+");

  tabulate::Table consecutive;
  consecutive.add_row({""});
  consecutive.add_row({""});
  t.expect("consecutive empty rows", consecutive, "+--+\n|  |\n+--+\n|  |\n+--+");

  tabulate::Table padded;
  padded.add_row({""});
  padded[0].format().padding_top(1).padding_bottom(2);
  t.expect("padding surrounds an empty content line", padded,
           "+--+\n|  |\n|  |\n|  |\n|  |\n+--+");

  tabulate::Table zero_padding;
  zero_padding.add_row({""});
  zero_padding[0].format().padding(0);
  t.expect("zero-width empty cell", zero_padding, "++\n||\n++");

  tabulate::Table height;
  height.add_row({""});
  height[0].format().height(3);
  t.expect("explicit height is preserved", height, "+--+\n|  |\n|  |\n|  |\n+--+");

  tabulate::Table mixed;
  mixed.add_row({"", "value"});
  t.expect("mixed empty and nonempty cells", mixed, "+--+-------+\n|  | value |\n+--+-------+");

  tabulate::Table nonempty;
  nonempty.add_row({"value"});
  t.expect("nonempty control", nonempty, "+-------+\n| value |\n+-------+");

  tabulate::Table multiline;
  multiline.add_row({"a\nb"});
  t.expect("multiline control", multiline, "+---+\n| a |\n| b |\n+---+");

  tabulate::Table trailing;
  trailing.add_row({"a\n"});
  // Preserve the existing width calculation for trailing/newline-only text.
  t.expect("trailing newline control", trailing, "+----+\n| a  |\n+----+");

  tabulate::Table newline;
  newline.add_row({"\n"});
  t.expect("newline-only control", newline, "+---+\n|   |\n+---+");

  return t.report();
}
