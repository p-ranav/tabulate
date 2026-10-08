#ifdef TABULATE_SINGLE_HEADER
#include <tabulate/tabulate.hpp>
#else
#include <tabulate/table.hpp>
#include <tabulate/termcolor.hpp>
#endif

#if __cplusplus >= 201703L
#include <string_view>
#elif !defined(TABULATE_SINGLE_HEADER)
#include <tabulate/string_view_lite.hpp>
#endif
#include <iomanip>
#include <iostream>
#include <sstream>

#include "test_utils.hpp"

// Coverage for Table I/O surfaces beyond add_row(Row_t)/str()/print(): the
// free operator<<(ostream&, const Table&), string_view cells, rows shorter
// than the established column count, and RowStream (the std::ostream-style
// row builder used to add_row() non-string values like ints/doubles).
int main() {
  tabulate_test::Harness t;
  using tabulate::RowStream;
  using Row_t = tabulate::Table::Row_t;

#if __cplusplus >= 201703L
  using std::string_view;
#else
  using nonstd::string_view;
#endif

  {
    tabulate::Table table;
    table.add_row(Row_t{"x"});
    std::ostringstream out;
    out << table;
    t.expect_eq("operator<<(ostream&, Table&) prints the table", out.str(), table.str());
  }

  {
    string_view sv = "view";
    tabulate::Table table;
    table.add_row(Row_t{sv, "plain"});
    t.expect("a string_view cell renders like a std::string cell", table,
             "+------+-------+\n| view | plain |\n+------+-------+");
  }

  {
    // A row with fewer cells than the column count established by the first
    // row is padded with empty cells instead of crashing.
    tabulate::Table table;
    table.add_row(Row_t{"a", "b", "c"});
    table.add_row(Row_t{"x"});
    t.expect("a short row is padded with empty cells", table,
             "+---+---+---+\n| a | b | c |\n+---+---+---+\n| x |   |   |\n+---+---+---+");
  }

  {
    RowStream rs;
    rs << std::setprecision(4);
    tabulate::Table table;
    table.add_row(RowStream{}.copyfmt(rs) << 101 << "Donald" << 3.14159265);
    t.expect("RowStream streams ints/strings/doubles into a row", table,
             "+-----+--------+-------+\n| 101 | Donald | 3.142 |\n+-----+--------+-------+");
  }

  {
    // Streaming a manipulator-only value (no visible text) must not add an
    // empty cell to the row.
    RowStream rs;
    rs << std::setprecision(2) << 1.5;
    tabulate::Table table;
    table.add_row(rs);
    t.expect("a formatting-only manipulator doesn't add a cell", table, "+-----+\n| 1.5 |\n+-----+");
  }

  {
    // Exercise RowStream's std::ostringstream-forwarding methods directly.
    RowStream rs;
    rs.precision(3);
    t.expect_eq("RowStream::precision() reads back what was set",
                std::to_string(rs.precision()), "3");
    rs.width(5);
    t.expect_eq("RowStream::width() reads back what was set", std::to_string(rs.width()), "5");
    rs.fill('*');
    t.expect_eq("RowStream::fill() reads back what was set", std::string(1, rs.fill()), "*");
    rs.flags(std::ios::fixed);
    t.expect_eq("RowStream::flags() reads back what was set",
                (rs.flags() & std::ios::fixed) ? "fixed" : "not-fixed", "fixed");
    rs.setf(std::ios::showpos);
    t.expect_eq("RowStream::setf(flags) sets a flag",
                (rs.flags() & std::ios::showpos) ? "showpos" : "not-showpos", "showpos");
    rs.unsetf(std::ios::showpos);
    t.expect_eq("RowStream::unsetf() clears a flag",
                (rs.flags() & std::ios::showpos) ? "showpos" : "not-showpos", "not-showpos");
    rs.setf(std::ios::hex, std::ios::basefield);
    t.expect_eq("RowStream::setf(flags, mask) sets within a mask",
                (rs.flags() & std::ios::basefield) == std::ios::hex ? "hex" : "not-hex", "hex");
    rs.exceptions(std::ios::goodbit);
    t.expect_eq("RowStream::exceptions() reads back what was set",
                rs.exceptions() == std::ios::goodbit ? "goodbit" : "other", "goodbit");
    std::locale loc = rs.getloc();
    rs.imbue(loc);
    t.expect_eq("RowStream::imbue()/getloc() round-trip without crashing", "ok", "ok");
    t.expect_eq("RowStream::narrow() forwards to the underlying stream",
                std::string(1, rs.narrow('A', '?')), "A");
    t.expect_eq("RowStream::widen() forwards to the underlying stream", rs.widen('A') == L'A' ? "A" : "?",
                "A");

    RowStream other;
    std::ostringstream ios_source;
    ios_source.precision(6);
    other.copyfmt(ios_source);
    t.expect_eq("RowStream::copyfmt(const std::ios&) copies format state",
                std::to_string(other.precision()), "6");

    RowStream another;
    another.copyfmt(rs);
    t.expect_eq("RowStream::copyfmt(const RowStream&) copies format state",
                std::to_string(another.precision()), "3");
  }

  {
    // When std::cout has been marked colorized (its iword flag set, as
    // termcolor::colorize does), adding a nested table as a cell must
    // propagate that colorization to the nested table's own stream so its
    // color codes aren't silently stripped.
    std::cout << termcolor::colorize;
    tabulate::Table inner;
    inner.add_row(Row_t{"key", "value"});
    inner[0][0].format().font_color(tabulate::Color::red);

    tabulate::Table outer;
    outer.add_row(Row_t{inner});
    std::cout << termcolor::nocolorize; // don't leak this into other tests

    const std::string rendered = outer.str();
    t.expect_eq("a nested table inherits std::cout's colorize flag",
                rendered.find("\033[31m") != std::string::npos ? "colorized" : "plain", "colorized");
  }

  return t.report();
}
