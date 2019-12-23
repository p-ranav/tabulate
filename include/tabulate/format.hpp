#include <cstddef>
#include <optional>
#include <string>
#include <tabulate/color.hpp>
#include <tabulate/font_align.hpp>
#include <tabulate/font_style.hpp>
#include <vector>

namespace tabulate {

class Format {
public:
  Format &width(size_t value) {
    width_ = value;
    return *this;
  }

private:
  friend class Cell;
  friend class Row;
  friend class TableInternal;

  // Element width and height
  std::optional<size_t> width_{};
  std::optional<size_t> height_{};

  // Font styling
  std::optional<FontAlign> font_align_{};
  std::optional<std::vector<FontStyle>> font_style_{};
  std::optional<Color> font_color_{};
  std::optional<Color> font_background_color_{};

  // Margin styling
  std::optional<size_t> margin_left_{};
  std::optional<size_t> margin_top_{};
  std::optional<size_t> margin_right_{};
  std::optional<size_t> margin_bottom_{};

  // Element padding
  std::optional<size_t> padding_left_{};
  std::optional<size_t> padding_top_{};
  std::optional<size_t> padding_right_{};
  std::optional<size_t> padding_bottom_{};

  // Element border
  std::optional<std::string> border_top_{};
  std::optional<Color> border_top_color_{};
  std::optional<Color> border_top_background_color_{};

  std::optional<std::string> border_bottom_{};
  std::optional<Color> border_bottom_color_{};
  std::optional<Color> border_bottom_background_color_{};

  std::optional<std::string> border_left_{};
  std::optional<Color> border_left_color_{};
  std::optional<Color> border_left_background_color_{};

  std::optional<std::string> border_right_{};
  std::optional<Color> border_right_color_{};
  std::optional<Color> border_right_background_color_{};

  std::optional<std::string> corner_{};
  std::optional<Color> corner_color_{};
  std::optional<Color> corner_background_color_{};

  std::optional<std::string> column_separator_{};
  std::optional<Color> column_separator_color_{};
  std::optional<Color> column_separator_background_color_{};
};

}; // namespace tabulate
