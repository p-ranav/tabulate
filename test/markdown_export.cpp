#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#include <tabulate/markdown_exporter.hpp>
#endif

#include "test_utils.hpp"

int main() {
  tabulate_test::Harness t;
  using tabulate::FontAlign;
  using tabulate::MarkdownExporter;
  using Row_t = tabulate::Table::Row_t;

  {
    tabulate::Table table;
    table.add_row(Row_t{"Name", "Age"});
    table.add_row(Row_t{"Alice", "30"});
    table.column(1).format().font_align(FontAlign::right);
    MarkdownExporter exporter;
    t.expect_eq("markdown export produces a pipe table with an alignment row",
                exporter.dump(table), "| Name  |   Age |\n| :---- | ----: |\n| Alice |    30 |\n");
  }

  {
    // dump() must not permanently mutate the table's own formatting.
    tabulate::Table table;
    table.add_row(Row_t{"Name", "Age"});
    table.add_row(Row_t{"Alice", "30"});
    table.column(1).format().font_align(FontAlign::right);
    MarkdownExporter exporter;
    (void)exporter.dump(table);
    t.expect("table formatting is restored after markdown export", table,
             "+-------+-----+\n| Name  | Age |\n+-------+-----+\n| Alice |  30 |\n+-------+-----+");
  }

  {
    // dump() can be called more than once without corrupting state.
    tabulate::Table table;
    table.add_row(Row_t{"a"});
    MarkdownExporter exporter;
    std::string first = exporter.dump(table);
    std::string second = exporter.dump(table);
    t.expect_eq("markdown export is idempotent across repeated dump() calls", second, first);
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"only"});
    MarkdownExporter exporter;
    t.expect_eq("markdown export handles a single column", exporter.dump(table),
                "| only  |\n| :---- |\n");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"mid"});
    table.column(0).format().font_align(FontAlign::center);
    MarkdownExporter exporter;
    t.expect_eq("markdown export uses :---: for center-aligned columns", exporter.dump(table),
                "|  mid  |\n| :---: |\n");
  }

  return t.report();
}
