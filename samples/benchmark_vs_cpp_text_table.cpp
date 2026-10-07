// Benchmarks tabulate against haarcuba/cpp-text-table
// (https://github.com/haarcuba/cpp-text-table), LGPL-2.1 licensed.
//
// TextTable only supports plain cell strings, a single border style, and
// per-column (not per-cell) alignment -- it has no equivalent of tabulate's
// colors, font styles, word-wrapping, multi-byte width handling, or per-cell
// formatting. So this benchmark intentionally sticks to the common subset
// both libraries share: building a rows x cols table of plain strings and
// printing it to a stream. It says nothing about the cost of tabulate
// features TextTable simply can't do.
#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

#include <tabulate/table.hpp>
#include <TextTable.h>

using namespace tabulate;
using Row_t = Table::Row_t;

namespace {

// Same best-of-N timing methodology as benchmark.cpp: the minimum across
// repeats is the most stable estimate of true cost.
template <typename Fn> double time_ms(Fn &&fn, int repeats = 5) {
  double best = -1.0;
  for (int i = 0; i < repeats; ++i) {
    auto start = std::chrono::steady_clock::now();
    fn();
    double ms =
        std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start)
            .count();
    if (best < 0.0 || ms < best)
      best = ms;
  }
  return best;
}

std::string ms_str(double ms) {
  std::ostringstream out;
  out << std::fixed << std::setprecision(2) << ms << " ms";
  return out.str();
}

std::string ratio_str(double ms, double baseline_ms) {
  std::ostringstream out;
  out << std::fixed << std::setprecision(2) << (ms / baseline_ms) << "x";
  return out.str();
}

// Identical plain-text content for both libraries, so neither gets an unfair
// content-generation advantage inside the timed region.
std::vector<std::vector<std::string>> make_rows(int rows, int cols) {
  std::vector<std::vector<std::string>> data(rows);
  for (int r = 0; r < rows; ++r) {
    data[r].reserve(cols);
    for (int c = 0; c < cols; ++c)
      data[r].push_back("r" + std::to_string(r) + "c" + std::to_string(c));
  }
  return data;
}

double time_tabulate(const std::vector<std::vector<std::string>> &data) {
  return time_ms(
      [&] {
        Table table;
        for (const auto &row : data)
          table.add_row(Row_t(row.begin(), row.end()));
        std::ostringstream out;
        table.print(out);
      },
      3);
}

double time_text_table(const std::vector<std::vector<std::string>> &data) {
  return time_ms(
      [&] {
        TextTable table;
        for (const auto &row : data)
          table.addRow(row);
        std::ostringstream out;
        out << table;
      },
      3);
}

void benchmark_scaling(Table &results) {
  for (int rows : {100, 1000, 5000, 20000}) {
    for (int cols : {2, 5, 10}) {
      auto data = make_rows(rows, cols);
      double tabulate_ms = time_tabulate(data);
      double text_table_ms = time_text_table(data);
      long long cells = static_cast<long long>(rows) * cols;
      results.add_row(Row_t{std::to_string(rows), std::to_string(cols), std::to_string(cells),
                            ms_str(tabulate_ms), ms_str(text_table_ms),
                            ratio_str(tabulate_ms, text_table_ms)});
    }
  }
}

} // namespace

int main() {
  std::cout << "=== tabulate vs. cpp-text-table: plain rows x cols table, construct + print ===\n";
  std::cout << "(TextTable has no colors/styles/word-wrap/per-cell formatting, so only the\n"
            << " common subset -- plain text layout -- is compared)\n\n";

  Table results;
  results.add_row(Row_t{"Rows", "Cols", "Cells", "tabulate", "cpp-text-table", "tabulate/TextTable"});
  benchmark_scaling(results);
  for (size_t c = 3; c <= 5; ++c)
    results.column(c).format().font_align(FontAlign::right);
  std::cout << results << "\n";

  return 0;
}
