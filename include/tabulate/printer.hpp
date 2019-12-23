#pragma once
#include <vector>
#include <utility>

namespace tabulate {

class Printer {
public:
  static std::pair<std::vector<size_t>, std::vector<size_t>> compute_cell_dimensions(TableInternal& table);

  static void print_table(std::ostream& stream, TableInternal& table);

  static void print_cell(std::ostream& stream, TableInternal& table, const std::pair<size_t, size_t>& index, const std::pair<size_t, size_t>& dimension);

  static void print_cell_border_top(std::ostream& stream, TableInternal& table, const std::pair<size_t, size_t>& index, const std::pair<size_t, size_t>& dimension);

};

}