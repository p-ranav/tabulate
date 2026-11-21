#include <sstream>
#include <string>
#include <vector>

#include "../test_utils.hpp"

import tabulate;

int main() {
  tabulate_test::Harness t;

  {
    tabulate::Table table;
    table.add_row({"Name", "Score"});
    table.add_row(tabulate::RowStream{} << "Alice" << 42);
    table[0].format().font_style({tabulate::FontStyle::bold}).font_color(tabulate::Color::cyan);

    tabulate::Column score_col = table.column(1);
    tabulate::ColumnFormat col_fmt = score_col.format();
    col_fmt.font_align(tabulate::FontAlign::right);

    t.expect("basic table formatted via C++20 module", table,
             "+-------+-------+\n"
             "| Name  | Score |\n"
             "+-------+-------+\n"
             "| Alice |    42 |\n"
             "+-------+-------+");

    std::ostringstream oss;
    oss << table;
    t.expect_eq("stream operator<< works via ADL from module", oss.str(), table.str());
  }

  {
    tabulate::Table table;
    table.add_row({"A", "B"});
    table.add_row({"1", "2"});

    size_t cell_count = 0;
    for (tabulate::Row &row : table) {
      for (tabulate::Cell &cell : row) {
        if (!cell.get_text().empty())
          ++cell_count;
      }
    }
    t.expect_eq("explicit Row and Cell types work in range-for", std::to_string(cell_count), "4");
    t.expect_eq("paginate works via module", std::to_string(table.paginate(1).size()), "1");

    table.use_unicode_borders(false, tabulate::BorderStyle::Light);
    t.expect("BorderStyle enum and unicode borders work via module", table,
             "\u250c\u2500\u2500\u2500\u252c\u2500\u2500\u2500\u2510\n"
             "\u2502 A \u2502 B \u2502\n"
             "\u2502 1 \u2502 2 \u2502\n"
             "\u2514\u2500\u2500\u2500\u2534\u2500\u2500\u2500\u2518");
  }

  {
    tabulate::Format a;
    tabulate::Format b;
    a.trim_mode(tabulate::Format::TrimMode::kNone);
    t.expect_eq("Format::TrimMode and Format::operator!= work via module",
                a != b ? "different" : "same", "different");
  }

  {
    tabulate::Table table;
    table.add_row({"Col1", "Col2"});
    table.add_row({"x", "y"});

    tabulate::MarkdownExporter md;
    tabulate::Exporter &base_md = md;
    t.expect_eq("MarkdownExporter via polymorphic Exporter base", base_md.dump(table),
                "| Col1  | Col2  |\n| :---- | :---- |\n| x     | y     |\n");

    tabulate::LatexExporter latex;
    latex.configure().indentation(2);
    t.expect_eq("LatexExporter works via module", latex.dump(table),
                "\\begin{tabular}\n{ll}\n  Col1 & Col2 \\\\\n  x & y \\\\\n\\end{tabular}");

    tabulate::AsciiDocExporter asciidoc;
    t.expect_eq("AsciiDocExporter works via module", asciidoc.dump(table),
                "[cols=\"<,<\"]\n|===\n|Col1|Col2\n\n|x|y\n|===");
  }

  return t.report();
}
