#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include "test_utils.hpp"

// Regression tests for https://github.com/p-ranav/tabulate/issues/86: trim_mode
// controls whether leading/trailing whitespace on each line of a multi-line
// cell is stripped.
int main() {
  tabulate_test::Harness t;
  using tabulate::Format;
  using Row_t = tabulate::Table::Row_t;

  const std::string indented = "{\n  \"a\": 1\n}";

  {
    tabulate::Table table;
    table.add_row(Row_t{indented});
    // kBoth is the default: both ends of each line get trimmed. The column
    // width is computed from the untrimmed content, so trimmed lines are
    // left with trailing blank padding instead of shrinking the column.
    t.expect("default trim_mode (kBoth) strips leading indentation", table,
             "+----------+\n| {        |\n| \"a\": 1   |\n| }        |\n+----------+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{indented});
    table[0][0].format().trim_mode(Format::TrimMode::kNone);
    t.expect("trim_mode(kNone) preserves leading indentation", table,
             "+----------+\n| {        |\n|   \"a\": 1 |\n| }        |\n+----------+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"  padded  "});
    table[0][0].format().width(12).trim_mode(Format::TrimMode::kLeft);
    t.expect("trim_mode(kLeft) strips only the leading whitespace", table,
             "+------------+\n| padded     |\n+------------+");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"  padded  "});
    table[0][0].format().width(12).trim_mode(Format::TrimMode::kRight);
    t.expect("trim_mode(kRight) strips only the trailing whitespace", table,
             "+------------+\n|   padded   |\n+------------+");
  }

  return t.report();
}
