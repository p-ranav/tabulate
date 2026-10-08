#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#include <tabulate/asciidoc_exporter.hpp>
#endif

#include "test_utils.hpp"

int main() {
  tabulate_test::Harness t;
  using tabulate::FontAlign;
  using tabulate::FontStyle;
  using tabulate::AsciiDocExporter;
  using Row_t = tabulate::Table::Row_t;

  {
    tabulate::Table table;
    table.add_row(Row_t{"Name", "Age"});
    table.add_row(Row_t{"Alice", "30"});
    table.column(1).format().font_align(FontAlign::right);
    AsciiDocExporter exporter;
    t.expect_eq("asciidoc export emits a cols header and pipe-delimited rows", exporter.dump(table),
                "[cols=\"<,>\"]\n|===\n|Name|Age\n\n|Alice|30\n|===");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().font_style({FontStyle::bold});
    AsciiDocExporter exporter;
    t.expect_eq("asciidoc export wraps bold cells in *asterisks*", exporter.dump(table),
                "[cols=\"<\"]\n|===\n|*x*\n\n|===");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().font_style({FontStyle::italic});
    AsciiDocExporter exporter;
    t.expect_eq("asciidoc export wraps italic cells in _underscores_", exporter.dump(table),
                "[cols=\"<\"]\n|===\n|_x_\n\n|===");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table[0][0].format().font_style({FontStyle::bold, FontStyle::italic});
    AsciiDocExporter exporter;
    t.expect_eq("asciidoc export combines bold and italic as *_x_*", exporter.dump(table),
                "[cols=\"<\"]\n|===\n|*_x_*\n\n|===");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"a", "b", "c"});
    table.column(0).format().font_align(FontAlign::left);
    table.column(1).format().font_align(FontAlign::center);
    table.column(2).format().font_align(FontAlign::right);
    AsciiDocExporter exporter;
    t.expect_eq("asciidoc export's cols header has one marker per column", exporter.dump(table),
                "[cols=\"<,^,>\"]\n|===\n|a|b|c\n\n|===");
  }

  return t.report();
}
