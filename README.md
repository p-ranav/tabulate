<p align="center">
  <img height="50" src="img/logo.jpg"/>  
</p>

<p align="center">
  <img src="img/summary.png"/>  
  <p align="center">
   Source for the above image can be found
    <a href="https://github.com/p-ranav/tabulate/blob/master/samples/summary.cpp">
      here
    </a>
  </p>
</p>

## Quick Start

`tabulate` is a header-only library. Just add `include/` to your `include_directories` and you should be good to go. To build a table, create a `Table` object and use `Table.add_rows` to add rows to your table. 

For simplicity, `tabulate` will use the length of the first row as the number of columns in the table.

```cpp
#include <tabulate/table.hpp>
using namespace tabulate;

int main() {

  Table universal_constants;

  universal_constants.add_row({"Quantity", "Value"});
  universal_constants.add_row({"Characteristic impedance of vacuum", "376.730 313 461... Ω"});
  universal_constants.add_row({"Electric constant (permittivity of free space)", "8.854 187 817... × 10⁻¹²F·m⁻¹"});
  universal_constants.add_row({"Magnetic constant (permeability of free space)", "4π × 10⁻⁷ N·A⁻² = 1.2566 370 614... × 10⁻⁶ N·A⁻²"});
  universal_constants.add_row({"Gravitational constant (Newtonian constant of gravitation)", "6.6742(10) × 10⁻¹¹m³·kg⁻¹·s⁻²"});
  universal_constants.add_row({"Planck's constant", "6.626 0693(11) × 10⁻³⁴ J·s"});
  universal_constants.add_row({"Dirac's constant", "1.054 571 68(18) × 10⁻³⁴ J·s"});
  universal_constants.add_row({"Speed of light in vacuum", "299 792 458 m·s⁻¹"});
```

You can format this table using `Table.format()` which returns a `Format` object. Using a fluent interface, format properties of the table, e.g., borders, font styles, colors etc.

```cpp
  universal_constants.format()
    .font_style({FontStyle::bold})
    .border_top(" ")
    .border_bottom(" ")
    .border_left(" ")
    .border_right(" ")
    .corner(" ");
```

You can access rows in the table using `Table[row_index]`. This will return a `Row` object on which you can similarly call `Row.format()` to format its properties. Now, let's format the header of the table. The following code changes the font background of the header row to `red`, aligns the cell contents to `center` and applied a padding to the top and bottom of the row.

```cpp
  universal_constants[0].format()
    .padding_top(1)
    .padding_bottom(1)
    .font_align(FontAlign::center)
    .font_style({FontStyle::underline})
    .font_background_color(Color::red);
```

Use `Table.column(index)` will return a `Column` object. Columns store `reference_wrapper`s to cells in the table. You can use `Column.format()` to format all the cells in that column.

```cpp
  universal_constants.column(1).format()
    .font_color(Color::cyan);

  std::cout << universal_constants << std::endl;
}
```

<p align="center">
  <img src="img/universal_constants.jpg"/>  
</p>

## Inheritance Model to Table Formatting

Formatting in `tabulate` follows a simple inheritance model:

* `Table`, `Row` and `Cell` objects each have their own `Format` object.
* `Cell` format has highest precedence and `Table` format has lowest precedence.
* If cell-level formatting is provided, then for those cells, cell-level formatting is used.
* If cell-level formatting is not provided, its parent row-level formatting is used.
* If no row-level formatting is provided, its parent table-level formatting is used.
