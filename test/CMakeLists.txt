# Builds `<name>.cpp` twice: once against the modular headers (linked via
# tabulate::tabulate) and once against the generated single_include header
# (built without that target, so include/ isn't on the search path ahead of
# single_include/). Registers both as CTest tests. Executable targets are
# prefixed with "test_" so they don't collide with same-named sample targets
# when SAMPLES and tabulate_BUILD_TESTS are both enabled.
function(add_tabulate_test name)
  add_executable(test_${name} ${name}.cpp)
  target_link_libraries(test_${name} PRIVATE tabulate::tabulate)
  add_test(NAME ${name} COMMAND test_${name})

  add_executable(test_${name}_single_header ${name}.cpp)
  target_compile_definitions(test_${name}_single_header PRIVATE TABULATE_SINGLE_HEADER)
  target_include_directories(test_${name}_single_header PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../single_include)
  target_compile_features(test_${name}_single_header PRIVATE cxx_std_11)
  add_test(NAME ${name}_single_header COMMAND test_${name}_single_header)
endfunction()

add_tabulate_test(empty_rows)
add_tabulate_test(format_propagation)
add_tabulate_test(nested_tables)
add_tabulate_test(column_operations)
add_tabulate_test(alignment_and_wrapping)
add_tabulate_test(trim_mode)
add_tabulate_test(iterators)
add_tabulate_test(border_corner_styles)
add_tabulate_test(unicode_borders)
add_tabulate_test(multi_byte_wrapping)
add_tabulate_test(no_global_namespace_pollution)
add_tabulate_test(black_color)
add_tabulate_test(table_dimensions)
