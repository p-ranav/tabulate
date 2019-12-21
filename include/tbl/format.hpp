#include <cstddef>
#include <optional>
#include <string>
#include <vector>
#include <tbl/color.hpp>
#include <tbl/font_align.hpp>
#include <tbl/font_style.hpp>

namespace tbl {

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

  Format &border_left(const std::string &value) {
    border_left_ = value;
    return *this;
  }

  Format &border_right(const std::string &value) {
    border_right_ = value;
    return *this;
  }

  Format &border_top(const std::string &value) {
    border_top_ = value;
    return *this;
  }

  Format &border_bottom(const std::string &value) {
    border_bottom_ = value;
    return *this;
  }

  Format &corner(const std::string &value) {
    corner_ = value;
    return *this;
  }

  Format &column_separator(const std::string &value) {
    column_separator_ = value;
    return *this;
  }

  Format &font_align(FontAlign value) {
    font_align_ = value;
    return *this;
  }

  Format &font_style(const std::vector<FontStyle> &style) {
    for (auto &s : style)
      font_style_.push_back(s);
    return *this;
  }

  Format &color(Color value) {
    color_ = value;
    return *this;
  }

  Format &background_color(Color value) {
    background_color_ = value;
    return *this;
  }

  Format &word_wrap(bool value) {
    word_wrap_ = value;
    return *this;
  }

private:
  friend class Cell;
  friend class Row;
  friend class Table;
  std::optional<size_t> width_{};
  std::optional<size_t> height_{};

  FontAlign font_align_{FontAlign::left};
  std::vector<FontStyle> font_style_{};

  std::optional<Color> color_;
  std::optional<Color> background_color_;

  size_t margin_left_{1};
  size_t margin_top_{1};
  size_t margin_right_{1};
  size_t margin_bottom_{1};

  size_t padding_left_{1};
  size_t padding_top_{1};
  size_t padding_right_{1};
  size_t padding_bottom_{1};

  std::string border_top_{"-"};
  std::string border_bottom_{"-"};
  std::string border_left_{"|"};
  std::string border_right_{"|"};

  std::string corner_{"+"};
  std::string column_separator_{"|"};

  bool word_wrap_{false};
};

}; // namespace tbl
