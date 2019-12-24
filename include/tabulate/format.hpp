#pragma once
#include <algorithm>
#include <cstddef>
#include <optional>
#include <sstream>
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

  // Apply word wrap
  // Given an input string and a line length, this will insert \n
  // in strategic places in input string and apply word wrapping
  static std::string word_wrap(const std::string &text, size_t line_length) {
    std::istringstream words(text);
    std::ostringstream wrapped;
    std::string word;

    if (words >> word) {
      wrapped << word;
      size_t space_left = line_length - word.length();
      while (words >> word) {
        if (space_left < word.length() + 1) {
          wrapped << '\n' << word;
          space_left = line_length - word.length();
        } else {
          wrapped << ' ' << word;
          space_left -= word.length() + 1;
        }
      }
    }
    return wrapped.str();
  }

  // Merge two formats
  // first has higher precedence
  // e.g., first = cell-level formatting and
  // second = row-level formatting
  // Result has attributes of both with cell-level
  // formatting taking precedence
  static Format merge(Format first, Format second) {
    Format result;

    // Width and height
    if (first.width_.has_value())
      result.width_ = first.width_;
    else
      result.width_ = second.width_;

    if (first.height_.has_value())
      result.height_ = first.height_;
    else
      result.height_ = second.height_;

    // Font styling
    if (first.font_align_.has_value())
      result.font_align_ = first.font_align_;
    else
      result.font_align_ = second.font_align_;

    if (first.font_style_.has_value()) {
      // Merge font styles using std::set_union
      std::vector<FontStyle> merged_font_style(first.font_style_.value().size() +
                                               second.font_style_.value().size());
      std::set_union(first.font_style_.value().begin(), first.font_style_.value().end(),
                     second.font_style_.value().begin(), second.font_style_.value().end(),
                     merged_font_style.begin());
      result.font_style_ = merged_font_style;
    } else
      result.font_style_ = second.font_style_;

    if (first.font_color_.has_value())
      result.font_color_ = first.font_color_;
    else
      result.font_color_ = second.font_color_;

    if (first.font_background_color_.has_value())
      result.font_background_color_ = first.font_background_color_;
    else
      result.font_background_color_ = second.font_background_color_;

    // Margin styling
    if (first.margin_left_.has_value())
      result.margin_left_ = first.margin_left_;
    else
      result.margin_left_ = second.margin_left_;

    if (first.margin_top_.has_value())
      result.margin_top_ = first.margin_top_;
    else
      result.margin_top_ = second.margin_top_;

    if (first.margin_right_.has_value())
      result.margin_right_ = first.margin_right_;
    else
      result.margin_right_ = second.margin_right_;

    if (first.margin_bottom_.has_value())
      result.margin_bottom_ = first.margin_bottom_;
    else
      result.margin_bottom_ = second.margin_bottom_;

    // Padding
    if (first.padding_left_.has_value())
      result.padding_left_ = first.padding_left_;
    else
      result.padding_left_ = second.padding_left_;

    if (first.padding_top_.has_value())
      result.padding_top_ = first.padding_top_;
    else
      result.padding_top_ = second.padding_top_;

    if (first.padding_right_.has_value())
      result.padding_right_ = first.padding_right_;
    else
      result.padding_right_ = second.padding_right_;

    if (first.padding_bottom_.has_value())
      result.padding_bottom_ = first.padding_bottom_;
    else
      result.padding_bottom_ = second.padding_bottom_;

    // Border
    if (first.border_left_.has_value())
      result.border_left_ = first.border_left_;
    else
      result.border_left_ = second.border_left_;

    if (first.border_left_color_.has_value())
      result.border_left_color_ = first.border_left_color_;
    else
      result.border_left_color_ = second.border_left_color_;

    if (first.border_left_background_color_.has_value())
      result.border_left_background_color_ = first.border_left_background_color_;
    else
      result.border_left_background_color_ = second.border_left_background_color_;

    if (first.border_top_.has_value())
      result.border_top_ = first.border_top_;
    else
      result.border_top_ = second.border_top_;

    if (first.border_top_color_.has_value())
      result.border_top_color_ = first.border_top_color_;
    else
      result.border_top_color_ = second.border_top_color_;

    if (first.border_top_background_color_.has_value())
      result.border_top_background_color_ = first.border_top_background_color_;
    else
      result.border_top_background_color_ = second.border_top_background_color_;

    if (first.border_bottom_.has_value())
      result.border_bottom_ = first.border_bottom_;
    else
      result.border_bottom_ = second.border_bottom_;

    if (first.border_bottom_color_.has_value())
      result.border_bottom_color_ = first.border_bottom_color_;
    else
      result.border_bottom_color_ = second.border_bottom_color_;

    if (first.border_bottom_background_color_.has_value())
      result.border_bottom_background_color_ = first.border_bottom_background_color_;
    else
      result.border_bottom_background_color_ = second.border_bottom_background_color_;

    if (first.border_right_.has_value())
      result.border_right_ = first.border_right_;
    else
      result.border_right_ = second.border_right_;

    if (first.border_right_color_.has_value())
      result.border_right_color_ = first.border_right_color_;
    else
      result.border_right_color_ = second.border_right_color_;

    if (first.border_right_background_color_.has_value())
      result.border_right_background_color_ = first.border_right_background_color_;
    else
      result.border_right_background_color_ = second.border_right_background_color_;

    // Corner
    if (first.corner_.has_value())
      result.corner_ = first.corner_;
    else
      result.corner_ = second.corner_;

    if (first.corner_color_.has_value())
      result.corner_color_ = first.corner_color_;
    else
      result.corner_color_ = second.corner_color_;

    if (first.corner_background_color_.has_value())
      result.corner_background_color_ = first.corner_background_color_;
    else
      result.corner_background_color_ = second.corner_background_color_;

    // Column separator
    if (first.column_separator_.has_value())
      result.column_separator_ = first.column_separator_;
    else
      result.column_separator_ = second.column_separator_;

    if (first.column_separator_color_.has_value())
      result.column_separator_color_ = first.column_separator_color_;
    else
      result.column_separator_color_ = second.column_separator_color_;

    if (first.column_separator_background_color_.has_value())
      result.column_separator_background_color_ = first.column_separator_background_color_;
    else
      result.column_separator_background_color_ = second.column_separator_background_color_;

    return result;
  }

private:
  friend class Cell;
  friend class Row;
  friend class Column;
  friend class TableInternal;
  friend class Printer;

  void set_defaults() {
    // NOTE: width and height are not set here
    font_align_ = FontAlign::left;
    font_style_ = std::vector<FontStyle>{};
    font_color_ = font_background_color_ = Color::none;
    margin_left_ = margin_right_ = margin_top_ = margin_bottom_ = 0;
    padding_left_ = padding_right_ = 1;
    padding_top_ = padding_bottom_ = 0;
    border_top_ = border_bottom_ = "-";
    border_left_ = border_right_ = "|";
    border_top_color_ = border_top_background_color_ = border_bottom_color_ =
        border_bottom_background_color_ = border_left_color_ = border_left_background_color_ =
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
