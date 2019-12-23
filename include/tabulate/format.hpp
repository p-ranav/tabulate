#pragma once
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

  Format &height(size_t value) {
    height_ = value;
    return *this;
  }

private:
  friend class Cell;
  friend class Row;
  friend class TableInternal;

  void set_defaults() {
    width_ = height_ = 1;
    font_align_ = FontAlign::left;
    font_style_ = std::vector<FontStyle>{};
    font_color_ = font_background_color_ = Color::none;
    margin_left_ = margin_right_ = margin_top_ = margin_bottom_ = 0;
    padding_left_ = padding_right_ = 1;
    padding_top_ = padding_bottom_ = 0;
    border_top_ = border_bottom_ = "-";
    border_left_ = border_right_ = "|";
    border_top_color_ = border_top_background_color_ =
      border_bottom_color_ = border_bottom_background_color_ =
      border_left_color_ = border_left_background_color_ =
      border_right_color_ = border_right_background_color_ = Color::none;
    corner_ = "+";
    corner_color_ = corner_background_color_ = Color::none;
    column_separator_ = "|";
    column_separator_color_ = column_separator_background_color_ = Color::none; 
  }

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
