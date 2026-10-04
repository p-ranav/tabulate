#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#endif

#include <sstream>

#include "test_utils.hpp"

// Regression tests for https://github.com/p-ranav/tabulate/issues/32's
// pagination follow-up: Table::paginate() splits a large table into
// consistently-formatted pages for printing.
int main() {
  tabulate_test::Harness t;
  using Row_t = tabulate::Table::Row_t;

  {
    // Evenly divisible: 9 data rows / 3 per page = exactly 3 pages, each
    // repeating the header row by default.
    tabulate::Table table;
    table.add_row(Row_t{"ID", "Name"});
    for (int i = 1; i <= 9; ++i)
      table.add_row(Row_t{std::to_string(i), "Item " + std::to_string(i)});

    auto pages = table.paginate(3);
    t.expect_eq("evenly divisible input produces the expected page count", std::to_string(pages.size()),
                "3");
    for (size_t p = 0; p < pages.size(); ++p)
      t.expect_eq(("page " + std::to_string(p) + " has header + 3 data rows").c_str(),
                  std::to_string(pages[p].size()), "4");

    t.expect_eq("page 0 contains rows 1-3", pages[0].str(),
                "+----+--------+\n| ID | Name   |\n+----+--------+\n| 1  | Item 1 |\n"
                "+----+--------+\n| 2  | Item 2 |\n+----+--------+\n| 3  | Item 3 |\n+----+--------+");
    t.expect_eq("page 1 contains rows 4-6", pages[1].str(),
                "+----+--------+\n| ID | Name   |\n+----+--------+\n| 4  | Item 4 |\n"
                "+----+--------+\n| 5  | Item 5 |\n+----+--------+\n| 6  | Item 6 |\n+----+--------+");
    t.expect_eq("page 2 contains rows 7-9", pages[2].str(),
                "+----+--------+\n| ID | Name   |\n+----+--------+\n| 7  | Item 7 |\n"
                "+----+--------+\n| 8  | Item 8 |\n+----+--------+\n| 9  | Item 9 |\n+----+--------+");
  }

  {
    // Uneven division: 10 data rows / 3 per page = 3 full pages + 1 page of
    // just 1 row; no padding, no error.
    tabulate::Table table;
    table.add_row(Row_t{"ID", "Name"});
    for (int i = 1; i <= 10; ++i)
      table.add_row(Row_t{std::to_string(i), "Item " + std::to_string(i)});

    auto pages = table.paginate(3);
    t.expect_eq("uneven division produces 4 pages", std::to_string(pages.size()), "4");
    t.expect_eq("last page has only the header + 1 remaining row",
                std::to_string(pages.back().size()), "2");
  }

  {
    // repeat_header_row = false: pages are pure contiguous slices, no header
    // duplication.
    tabulate::Table table;
    table.add_row(Row_t{"ID", "Name"});
    for (int i = 1; i <= 6; ++i)
      table.add_row(Row_t{std::to_string(i), "Item " + std::to_string(i)});

    auto pages = table.paginate(2, /*repeat_header_row=*/false);
    // 7 total rows (including row 0, now treated as plain data) / 2 per page
    // = 4 pages (2, 2, 2, 1).
    t.expect_eq("repeat_header_row=false paginates all rows as plain data",
                std::to_string(pages.size()), "4");
    t.expect_eq("first page with repeat_header_row=false starts with the actual row 0",
                pages[0].str(), "+----+--------+\n| ID | Name   |\n+----+--------+\n| 1  | Item 1 |\n"
                               "+----+--------+");
  }

  {
    // The key promise of paginate(): column widths are computed once from
    // the *whole* table, so every page lines up identically even though
    // page 0's content is much shorter than page 1's.
    tabulate::Table table;
    table.add_row(Row_t{"ID", "Name"});
    table.add_row(Row_t{"1", "x"});
    table.add_row(Row_t{"2", "a much much longer name than the others"});

    auto pages = table.paginate(1);
    t.expect_eq("pages share identical column widths regardless of per-page content",
                std::to_string(pages[0].str().find('\n')), std::to_string(pages[1].str().find('\n')));
  }

  {
    // Per-cell formatting (colors, styles, custom borders) set on the
    // original table is preserved on the corresponding page cell.
    tabulate::Table table;
    table.add_row(Row_t{"ID", "Name"});
    table.add_row(Row_t{"1", "red"});
    table[1][1].format().font_color(tabulate::Color::red);

    auto pages = table.paginate(1);
    std::stringstream stream;
    stream << termcolor::colorize;
    pages[0].print(stream);
    bool has_red = stream.str().find("\033[31m") != std::string::npos;
    t.expect_eq("per-cell formatting survives into the page", has_red ? "red" : "not-red", "red");
  }

  {
    // Table-level-only settings (e.g. indent) are carried over to every page.
    tabulate::Table table;
    table.add_row(Row_t{"ID", "Name"});
    table.add_row(Row_t{"1", "a"});
    table.format().indent(2);

    auto pages = table.paginate(1);
    t.expect_eq("table-level indent is preserved on each page", pages[0].str().substr(0, 2), "  ");
  }

  {
    // Edge cases: no pages when there's nothing to paginate.
    tabulate::Table empty_table;
    t.expect_eq("an empty table produces no pages", std::to_string(empty_table.paginate(10).size()),
                "0");

    tabulate::Table header_only;
    header_only.add_row(Row_t{"ID", "Name"});
    t.expect_eq("a header-only table (with repeat_header_row) produces no pages",
                std::to_string(header_only.paginate(10).size()), "0");

    tabulate::Table table;
    table.add_row(Row_t{"ID", "Name"});
    table.add_row(Row_t{"1", "a"});
    t.expect_eq("rows_per_page == 0 produces no pages", std::to_string(table.paginate(0).size()), "0");
  }

  {
    // paginate() must not mutate the original table.
    tabulate::Table table;
    table.add_row(Row_t{"ID", "Name"});
    for (int i = 1; i <= 5; ++i)
      table.add_row(Row_t{std::to_string(i), "Item " + std::to_string(i)});
    const std::string before = table.str();
    auto pages = table.paginate(2);
    t.expect_eq("the original table's rendering is unchanged after paginate()", table.str(), before);
  }

  return t.report();
}
