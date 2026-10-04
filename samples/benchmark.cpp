#include <chrono>
#include <functional>
#include <iomanip>
#include <sstream>
#include <string>
#include <tabulate/table.hpp>

#if __cplusplus >= 202002L && defined(__has_include)
#if __has_include(<format>)
#define TABULATE_BENCHMARK_HAS_STD_FORMAT 1
#include <format>
#endif
#endif

using namespace tabulate;
using Row_t = Table::Row_t;

namespace {

// Best-of-N timing: scheduling/cache jitter can only ever slow a run down,
// never speed it up, so the minimum across repeats is the most stable
// estimate of the true cost -- a single sample is too noisy to compare
// scenarios that differ by only a few percent.
template <typename Fn> double time_ms(Fn &&fn, int repeats = 5) {
  double best = -1.0;
  for (int i = 0; i < repeats; ++i) {
    auto start = std::chrono::steady_clock::now();
    fn();
    double ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - start).count();
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

// Builds a plain, unformatted rows x cols table with short placeholder text,
// used as the common base content for every feature-cost scenario below.
Table make_base_table(int rows, int cols) {
  Table table;
  for (int r = 0; r < rows; ++r) {
    Row_t row;
    for (int c = 0; c < cols; ++c)
      row.push_back("r" + std::to_string(r) + "c" + std::to_string(c));
    table.add_row(row);
  }
  return table;
}

std::string print_to_string(Table &table) {
  std::ostringstream out;
  table.print(out);
  return out.str();
}

// ============================================================================
// 1. Scaling: how does print() cost grow with table size (rows x columns),
//    with no formatting applied at all?
// ============================================================================
void benchmark_scaling(Table &results) {
  for (int rows : {100, 1000, 5000, 20000}) {
    for (int cols : {2, 5, 10}) {
      double ms = time_ms(
          [&] {
            Table table = make_base_table(rows, cols);
            print_to_string(table);
          },
          3);
      long long cells = static_cast<long long>(rows) * cols;
      double us_per_cell = (ms * 1000.0) / static_cast<double>(cells);
      std::ostringstream us_str;
      us_str << std::fixed << std::setprecision(2) << us_per_cell << " us";
      results.add_row(Row_t{std::to_string(rows), std::to_string(cols), std::to_string(cells),
                            ms_str(ms), us_str.str()});
    }
  }
}

// ============================================================================
// 2. Feature cost: starting from the same plain rows x cols table, how much
//    does each individual tabulate feature add to print() time? Each
//    scenario changes exactly one thing relative to "Baseline", so "vs
//    Baseline" isolates that one feature's cost.
// ============================================================================
void benchmark_feature_costs(Table &results, int rows, int cols) {
  auto measure = [&](const char *name, const std::function<void(Table &)> &configure,
                     double baseline_ms) {
    double ms = time_ms([&] {
      Table table = make_base_table(rows, cols);
      configure(table);
      print_to_string(table);
    });
    results.add_row(Row_t{name, ms_str(ms),
                          baseline_ms > 0 ? ratio_str(ms, baseline_ms) : std::string("1.00x")});
    return ms;
  };

  double baseline_ms = measure(
      "Baseline (no formatting)", [](Table &) {}, 0);

  measure(
      "+ Font color (table-level, 1 color)",
      [](Table &table) { table.format().font_color(Color::red); }, baseline_ms);

  measure(
      "+ Font color (per-cell, 8 cycling colors)",
      [](Table &table) {
        static const Color colors[] = {Color::grey,    Color::red,  Color::green, Color::yellow,
                                       Color::blue,    Color::magenta, Color::cyan, Color::white};
        size_t i = 0;
        for (auto &row : table)
          for (auto &cell : row)
            cell.format().font_color(colors[i++ % 8]);
      },
      baseline_ms);

  measure(
      "+ Background color (table-level, 1 color)",
      [](Table &table) { table.format().font_background_color(Color::blue); }, baseline_ms);

  measure(
      "+ Background color (per-cell, 8 cycling colors)",
      [](Table &table) {
        static const Color colors[] = {Color::grey,    Color::red,  Color::green, Color::yellow,
                                       Color::blue,    Color::magenta, Color::cyan, Color::white};
        size_t i = 0;
        for (auto &row : table)
          for (auto &cell : row)
            cell.format().font_background_color(colors[i++ % 8]);
      },
      baseline_ms);

  measure(
      "+ Font style bold (table-level)",
      [](Table &table) { table.format().font_style({FontStyle::bold}); }, baseline_ms);

  measure(
      "+ Font style (per-cell, 4 cycling styles)",
      [](Table &table) {
        static const FontStyle styles[] = {FontStyle::bold, FontStyle::italic, FontStyle::underline,
                                           FontStyle::dark};
        size_t i = 0;
        for (auto &row : table)
          for (auto &cell : row)
            cell.format().font_style({styles[i++ % 4]});
      },
      baseline_ms);

  measure(
      "+ Custom border/corner chars+colors (table-level)",
      [](Table &table) {
        table.format().border_color(Color::cyan).corner_color(Color::yellow).border("*").corner(
            "+");
      },
      baseline_ms);

  measure(
      "+ Row separators (show_row_separator)",
      [](Table &table) { table.format().show_row_separator(); }, baseline_ms);

  measure(
      "+ multi_byte_characters(true) (ASCII content)",
      [](Table &table) { table.format().multi_byte_characters(true); }, baseline_ms);

  measure(
      "+ Unicode box borders (use_unicode_borders)",
      [](Table &table) { table.use_unicode_borders(true); }, baseline_ms);

  double wrap_baseline_ms = time_ms([&] {
    Table table = make_base_table(rows, cols);
    print_to_string(table);
  });
  double wrap_ms = time_ms([&] {
    Table table;
    for (int r = 0; r < rows; ++r) {
      Row_t row;
      for (int c = 0; c < cols; ++c)
        row.push_back("This is a much longer piece of cell content for row " + std::to_string(r) +
                     " column " + std::to_string(c) +
                     " that will not fit on one line and must be word-wrapped");
      table.add_row(row);
    }
    table.column(0).format().width(20);
    print_to_string(table);
  });
  results.add_row(Row_t{"+ Long text forcing word-wrap (width=20)", ms_str(wrap_ms),
                        ratio_str(wrap_ms, wrap_baseline_ms)});

  measure(
      "Everything combined (colors+styles+borders+multibyte+separators)",
      [](Table &table) {
        static const Color colors[] = {Color::red, Color::green, Color::yellow, Color::blue};
        static const FontStyle styles[] = {FontStyle::bold, FontStyle::italic};
        size_t i = 0;
        table.format()
            .multi_byte_characters(true)
            .show_row_separator()
            .border_color(Color::cyan)
            .corner_color(Color::yellow);
        for (auto &row : table)
          for (auto &cell : row) {
            cell.format().font_color(colors[i % 4]).font_style({styles[i % 2]});
            ++i;
          }
      },
      baseline_ms);
}

// Micro-benchmark for https://github.com/p-ranav/tabulate/issues/74: is
// std::format a faster way to build the padded/repeated strings the printer
// writes (cell alignment padding, repeated border characters), compared to
// the std::string concatenation tabulate currently uses?
void benchmark_padding_construction(Table &results) {
  const int iterations = 200000;
  const size_t width = 24;
  volatile size_t sink = 0; // prevents the optimizer from eliding the loop body

  double concat_ms = time_ms([&] {
    for (int i = 0; i < iterations; ++i) {
      std::string padded = std::string(width - 2, ' ') + "hi";
      sink += padded.size();
    }
  });
  results.add_row(Row_t{"std::string concatenation (current)", ms_str(concat_ms)});

#if defined(TABULATE_BENCHMARK_HAS_STD_FORMAT)
  double format_ms = time_ms([&] {
    for (int i = 0; i < iterations; ++i) {
      std::string padded = std::format("{:>{}}", "hi", width);
      sink += padded.size();
    }
  });
  results.add_row(Row_t{"std::format (C++20)", ms_str(format_ms)});
#else
  results.add_row(Row_t{"std::format (C++20)", "not available (compiled with < C++20)"});
#endif
  (void)sink;
}

// Builds a rows x cols table where every cell holds a long sentence that
// won't fit on one line at any reasonably narrow column width, used by the
// word-wrap benchmarks below. Varying row/column content keeps every cell
// text distinct (closer to real data than repeating the same string).
Table make_long_text_table(int rows, int cols) {
  Table table;
  for (int r = 0; r < rows; ++r) {
    Row_t row;
    for (int c = 0; c < cols; ++c)
      row.push_back("This is a much longer piece of cell content for row " + std::to_string(r) +
                   " column " + std::to_string(c) +
                   " that will not fit on one line and must be word-wrapped properly");
    table.add_row(row);
  }
  return table;
}

// ============================================================================
// 4a. Word-wrap scaling: for each rows x cols size, compare a table of short,
//     single-line cells against the same size table where every cell is long
//     enough to require word-wrapping (all columns fixed to a narrow width).
// ============================================================================
void benchmark_word_wrap_scaling(Table &results, int wrap_width) {
  for (int rows : {500, 2000, 5000}) {
    for (int cols : {2, 5, 10}) {
      double baseline_ms = time_ms(
          [&] {
            Table table = make_base_table(rows, cols);
            print_to_string(table);
          },
          3);

      double wrapped_ms = time_ms(
          [&] {
            Table table = make_long_text_table(rows, cols);
            for (int c = 0; c < cols; ++c)
              table.column(c).format().width(wrap_width);
            print_to_string(table);
          },
          3);

      long long cells = static_cast<long long>(rows) * cols;
      results.add_row(Row_t{std::to_string(rows), std::to_string(cols), std::to_string(cells),
                            ms_str(baseline_ms), ms_str(wrapped_ms),
                            ratio_str(wrapped_ms, baseline_ms)});
    }
  }
}

// ============================================================================
// 4b. Word-wrap width sweep: for a fixed table size, how does cost change as
//     the column gets narrower (forcing each cell to wrap into more lines)?
// ============================================================================
void benchmark_word_wrap_width_sweep(Table &results, int rows, int cols) {
  double widest_ms = -1.0;
  for (int width : {80, 40, 20, 10, 5}) {
    double ms = time_ms(
        [&] {
          Table table = make_long_text_table(rows, cols);
          for (int c = 0; c < cols; ++c)
            table.column(c).format().width(width);
          print_to_string(table);
        },
        3);
    if (widest_ms < 0.0)
      widest_ms = ms;
    results.add_row(
        Row_t{std::to_string(width), ms_str(ms), ratio_str(ms, widest_ms)});
  }
}

} // namespace

int main() {
  std::cout << "=== 1. Scaling: print() cost by table size (no formatting) ===\n";
  Table scaling_results;
  scaling_results.add_row(Row_t{"Rows", "Cols", "Cells", "Time", "Time/cell"});
  benchmark_scaling(scaling_results);
  scaling_results.column(3).format().font_align(FontAlign::right);
  scaling_results.column(4).format().font_align(FontAlign::right);
  std::cout << scaling_results << "\n\n";

  std::cout << "=== 2. Feature cost: what does each feature add, on a 2000x5 table? ===\n";
  Table feature_results;
  feature_results.add_row(Row_t{"Scenario", "Time", "vs Baseline"});
  benchmark_feature_costs(feature_results, 2000, 5);
  feature_results.column(1).format().font_align(FontAlign::right);
  feature_results.column(2).format().font_align(FontAlign::right);
  std::cout << feature_results << "\n\n";

  std::cout << "=== 3a. Word-wrap scaling: short single-line cells vs. all-wrapped cells"
            << " (width=20), by table size ===\n";
  Table wrap_scaling_results;
  wrap_scaling_results.add_row(
      Row_t{"Rows", "Cols", "Cells", "Baseline (no wrap)", "Wrapped", "Wrapped/Baseline"});
  benchmark_word_wrap_scaling(wrap_scaling_results, 20);
  wrap_scaling_results.column(3).format().font_align(FontAlign::right);
  wrap_scaling_results.column(4).format().font_align(FontAlign::right);
  wrap_scaling_results.column(5).format().font_align(FontAlign::right);
  std::cout << wrap_scaling_results << "\n\n";

  std::cout << "=== 3b. Word-wrap width sweep: 2000x5 table, narrower columns force more lines"
            << " per cell ===\n";
  Table wrap_width_results;
  wrap_width_results.add_row(Row_t{"Column width", "Time", "vs width=80"});
  benchmark_word_wrap_width_sweep(wrap_width_results, 2000, 5);
  wrap_width_results.column(1).format().font_align(FontAlign::right);
  wrap_width_results.column(2).format().font_align(FontAlign::right);
  std::cout << wrap_width_results << "\n\n";

  std::cout << "=== 4. Padding/repeated-string construction: std::string vs. std::format ===\n";
  Table format_results;
  format_results.add_row(Row_t{"Method", "Time"});
  benchmark_padding_construction(format_results);
  std::cout << format_results << std::endl;
}

