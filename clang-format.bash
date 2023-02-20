#!/usr/bin/env bash
find ./include ./samples -type f \
  \( -iname \*.cpp -o \
    \( -iname \*.hpp -a ! -name optional_lite.hpp -a ! -name variant_lite.hpp -a ! -name string_view_lite.hpp \) \) \
  | xargs clang-format -style=file -i
