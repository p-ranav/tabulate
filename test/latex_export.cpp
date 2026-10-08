#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#include <tabulate/latex_exporter.hpp>
#endif

#include "test_utils.hpp"

int main() {
  tabulate_test::Harness t;
  using tabulate::FontAlign;
  using tabulate::LatexExporter;
  using Row_t = tabulate::Table::Row_t;

  {
    tabulate::Table table;
    table.add_row(Row_t{"Name", "Age"});
    table.add_row(Row_t{"Alice", "30"});
    table.column(0).format().font_align(FontAlign::left);
    table.column(1).format().font_align(FontAlign::right);
    LatexExporter exporter;
    t.expect_eq("latex export wraps rows in a tabular environment", exporter.dump(table),
                "\\begin{tabular}\n{lr}\nName & Age \\\\\nAlice & 30 \\\\\n\\end{tabular}");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"only"});
    table.column(0).format().font_align(FontAlign::center);
    LatexExporter exporter;
    t.expect_eq("latex export handles a single column", exporter.dump(table),
                "\\begin{tabular}\n{c}\nonly \\\\\n\\end{tabular}");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"a", "b", "c"});
    table.column(0).format().font_align(FontAlign::left);
    table.column(1).format().font_align(FontAlign::center);
    table.column(2).format().font_align(FontAlign::right);
    LatexExporter exporter;
    t.expect_eq("latex export's alignment header has one letter per column", exporter.dump(table),
                "\\begin{tabular}\n{lcr}\na & b & c \\\\\n\\end{tabular}");
  }

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    table.column(0).format().font_align(FontAlign::left);
    LatexExporter exporter;
    exporter.configure().indentation(2);
    t.expect_eq("ExportOptions::indentation() prepends spaces to each row", exporter.dump(table),
                "\\begin{tabular}\n{l}\n  x \\\\\n\\end{tabular}");
  }

  return t.report();
}
