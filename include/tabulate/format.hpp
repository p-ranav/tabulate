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

  Format &margin(size_t value) {
    margin_left_ = value;
    margin_right_ = value;
    margin_top_ = value;
    margin_bottom_ = value;
    return *this;
  }

  Format &margin_left(size_t value) {
    margin_left_ = value;
    return *this;
  }

  Format &margin_right(size_t value) {
    margin_right_ = value;
    return *this;
  }

  Format &margin_top(size_t value) {
    margin_top_ = value;
    return *this;
  }

  Format &margin_bottom(size_t value) {
    margin_bottom_ = value;
    return *this;
  }

  Format &padding(size_t value) {
    padding_left_ = value;
    padding_right_ = value;
    padding_top_ = value;
    padding_bottom_ = value;
    return *this;
  }

  Format &padding_left(size_t value) {
    padding_left_ = value;
    return *this;
  }

  Format &padding_right(size_t value) {
    padding_right_ = value;
    return *this;
  }

  Format &padding_top(size_t value) {
    padding_top_ = value;
    return *this;
  }

  Format &padding_bottom(size_t value) {
    padding_bottom_ = value;
    return *this;
  }

  Format &border(const std::string &value) {
    border_left_ = value;
    border_right_ = value;
    border_top_ = value;
    border_bottom_ = value;
    return *this;
  }

  Format &border_color(Color value) {
    border_left_color_ = value;
    border_right_color_ = value;
    border_top_color_ = value;
    border_bottom_color_ = value;    
    return *this;
  }

  Format &border_background_color(Color value) {
    border_left_background_color_ = value;
    border_right_background_color_ = value;
    border_top_background_color_ = value;
    border_bottom_background_color_ = value;    
    return *this;
  }  

  Format &border_left(const std::string &value) {
    border_left_ = value;
    return *this;
  }

  Format &border_left_color(Color value) {
    border_left_color_ = value;
    return *this;
  }

  Format &border_left_background_color(Color value) {
    border_left_background_color_ = value;
    return *this;
  }      

  Format &border_right(const std::string &value) {
    border_right_ = value;
    return *this;
  }

  Format &border_right_color(Color value) {
    border_right_color_ = value;
    return *this;
  }

  Format &border_right_background_color(Color value) {
    border_right_background_color_ = value;
    return *this;
  }        

  Format &border_top(const std::string &value) {
    border_top_ = value;
    return *this;
  }

  Format &border_top_color(Color value) {
    border_top_color_ = value;
    return *this;
  }

  Format &border_top_background_color(Color value) {
    border_top_background_color_ = value;
    return *this;
  }    

  Format &border_bottom(const std::string &value) {
    border_bottom_ = value;
    return *this;
  }

  Format &border_bottom_color(Color value) {
    border_bottom_color_ = value;
    return *this;
  }

  Format &border_bottom_background_color(Color value) {
    border_bottom_background_color_ = value;
    return *this;
  }          

  Format &corner(const std::string &value) {
    corner_ = value;
    return *this;
  }

  Format &corner_color(Color value) {
    corner_color_ = value;
    return *this;
  }

  Format &corner_background_color(Color value) {
    corner_background_color_ = value;
    return *this;
  }            

  Format &column_separator(const std::string &value) {
    column_separator_ = value;
    return *this;
  }

  Format &column_separator_color(Color value) {
    column_separator_color_ = value;
    return *this;
  }

  Format &column_separator_background_color(Color value) {
    column_separator_background_color_ = value;
    return *this;
  }  

  Format &font_align(FontAlign value) {
    font_align_ = value;
    return *this;
  }

  Format &font_style(const std::vector<FontStyle> &style) {
    if (font_style_.has_value()) {
      for (auto &s : style)
        font_style_.value().push_back(s);
    } else {
      font_style_ = style;
    }
    return *this;
  }

  Format &font_color(Color value) {
    font_color_ = value;
    return *this;
  }

  Format &font_background_color(Color value) {
    font_background_color_ = value;
    return *this;
  }

  Format &color(Color value) {
    font_color(value);
    border_color(value);
    corner_color(value);
    return *this;
  }

  Format &background_color(Color value) {
    font_background_color(value);
    border_background_color(value);
    corner_background_color(value);
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

  // Element corner
  std::optional<std::string> corner_{};
  std::optional<Color> corner_color_{};
  std::optional<Color> corner_background_color_{};

  // Element column separator
  std::optional<std::string> column_separator_{};
  std::optional<Color> column_separator_color_{};
  std::optional<Color> column_separator_background_color_{};
};

}; // namespace tabulate
