# Builds `<name>.cpp` twice: once against the modular headers (linked via
# tabulate::tabulate) and once against the generated single_include header
# (built without that target, so include/ isn't on the search path ahead of
# single_include/). Registers both as CTest tests.
function(add_tabulate_test name)
  add_executable(${name} ${name}.cpp)
  target_link_libraries(${name} PRIVATE tabulate::tabulate)
  add_test(NAME ${name} COMMAND ${name})

  add_executable(${name}_single_header ${name}.cpp)
  target_compile_definitions(${name}_single_header PRIVATE TABULATE_SINGLE_HEADER)
  target_include_directories(${name}_single_header PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../single_include)
  target_compile_features(${name}_single_header PRIVATE cxx_std_11)
  add_test(NAME ${name}_single_header COMMAND ${name}_single_header)
endfunction()

add_tabulate_test(empty_rows)
add_tabulate_test(format_propagation)
add_tabulate_test(nested_tables)
add_tabulate_test(column_operations)
add_tabulate_test(alignment_and_wrapping)
add_tabulate_test(trim_mode)
add_tabulate_test(iterators)
