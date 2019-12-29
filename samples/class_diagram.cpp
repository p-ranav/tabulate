#include <tabulate/table.hpp>
using namespace tabulate;

int main() {
  Table class_diagram;

  // Global styling
  class_diagram.format().font_style({FontStyle::bold}).font_align(FontAlign::center).width(60);

  // Animal class
  Table animal;
  animal.add_row({"Animal"});
  animal[0].format().font_align(FontAlign::center);

  // Animal properties nested table
  Table animal_properties;
  animal_properties.format().width(20);
  animal_properties.add_row({"+age: Int"});
  animal_properties.add_row({"+gender: String"});
  animal_properties[1].format().hide_border_top();

  // Animal methods nested table
  Table animal_methods;
  animal_methods.format().width(20);
  animal_methods.add_row({"+isMammal()"});
  animal_methods.add_row({"+mate()"});
  animal_methods[1].format().hide_border_top();

  animal.add_row({animal_properties});
  animal.add_row({animal_methods});
  animal[2].format().hide_border_top();

  class_diagram.add_row({animal});

  // Add rows in the class diagram for the up-facing arrow
  // THanks to center alignment, these will align just fine
  class_diagram.add_row({"▲"});
  class_diagram[1][0].format().hide_border_top().multi_byte_characters(true);
  class_diagram.add_row({"|"});
  class_diagram[2].format().hide_border_top();
  class_diagram.add_row({"|"});
  class_diagram[3].format().hide_border_top();

  // Duck class
  Table duck;
  duck.add_row({"Duck"});
  duck[0].format().font_align(FontAlign::center);

  // Duck proeperties nested table
  Table duck_properties;
  duck_properties.format().width(40);
  duck_properties.add_row({"+beakColor: String = \"yellow\""});

  // Duck methods nested table
  Table duck_methods;
  duck_methods.format().width(40);
  duck_methods.add_row({"+swim()"});
  duck_methods.add_row({"+quack()"});
  duck_methods[1].format().hide_border_top();

  duck.add_row({duck_properties});
  duck.add_row({duck_methods});
  duck[2].format().hide_border_top();

  class_diagram.add_row({duck});
  class_diagram[4].format().hide_border_top();

  std::cout << class_diagram << std::endl;
}