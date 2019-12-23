#pragma once
#include <utility>
#include <vector>

namespace tabulate {

class Printer {
public:
  static std::pair<std::vector<size_t>, std::vector<size_t>>
  compute_cell_dimensions(TableInternal &table);

  static void print_table(std::ostream &stream, TableInternal &table);

  static void print_row_in_cell(std::ostream &stream, TableInternal &table,
                                const std::pair<size_t, size_t> &index,
                                const std::pair<size_t, size_t> &dimension, size_t num_columns,
                                size_t row_index);

  static void print_cell_border_top(std::ostream &stream, TableInternal &table,
                                    const std::pair<size_t, size_t> &index,
                                    const std::pair<size_t, size_t> &dimension, size_t num_columns);
  static void print_cell_border_bottom(std::ostream &stream, TableInternal &table,
                                       const std::pair<size_t, size_t> &index,
                                       const std::pair<size_t, size_t> &dimension, size_t num_columns);
};

} // namespace tabulate