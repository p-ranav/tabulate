#pragma once
#include <vector>
#include <utility>

namespace tabulate {

class Printer {
public:
  static std::pair<std::vector<size_t>, std::vector<size_t>> compute_cell_dimensions(TableInternal& table);

  static void print(TableInternal& table, std::ostream& stream);

};

}