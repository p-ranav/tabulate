#pragma once

namespace tabulate {

class ColumnFormat : public Format {
public:
  ColumnFormat(class Column &column) : column_(column) {}

  Format &width(size_t value);
  Format &height(size_t value);

private:
  std::reference_wrapper<class Column> column_;
};

} // namespace tabulate