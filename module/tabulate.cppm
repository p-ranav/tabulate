/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

module;

#include <tabulate/asciidoc_exporter.hpp>
#include <tabulate/latex_exporter.hpp>
#include <tabulate/markdown_exporter.hpp>
#include <tabulate/table.hpp>

export module tabulate;

export namespace tabulate {
using tabulate::AsciiDocExporter;
using tabulate::BorderStyle;
using tabulate::Cell;
using tabulate::Color;
using tabulate::Column;
using tabulate::ColumnFormat;
using tabulate::Exporter;
using tabulate::FontAlign;
using tabulate::FontStyle;
using tabulate::Format;
using tabulate::LatexExporter;
using tabulate::MarkdownExporter;
using tabulate::Row;
using tabulate::RowStream;
using tabulate::Table;
using tabulate::operator<<;
} // namespace tabulate
