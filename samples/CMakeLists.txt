find_package(Threads REQUIRED)

add_executable(ansi_escape_width ansi_escape_width.cpp)
target_link_libraries(ansi_escape_width PRIVATE tabulate::tabulate)

add_executable(class_diagram class_diagram.cpp)
target_link_libraries(class_diagram PRIVATE tabulate::tabulate)

add_executable(colors colors.cpp)
target_link_libraries(colors PRIVATE tabulate::tabulate)

add_executable(employees employees.cpp)
target_link_libraries(employees PRIVATE tabulate::tabulate)

add_executable(font_styles font_styles.cpp)
target_link_libraries(font_styles PRIVATE tabulate::tabulate)

add_executable(iterators iterators.cpp)
target_link_libraries(iterators PRIVATE tabulate::tabulate)

add_executable(latex_export latex_export.cpp)
target_link_libraries(latex_export PRIVATE tabulate::tabulate)

add_executable(locale_preserved locale_preserved.cpp)
target_link_libraries(locale_preserved PRIVATE tabulate::tabulate)

add_executable(markdown_export markdown_export.cpp)
target_link_libraries(markdown_export PRIVATE tabulate::tabulate)

add_executable(mario mario.cpp)
target_link_libraries(mario PRIVATE tabulate::tabulate)

add_executable(movies movies.cpp)
target_link_libraries(movies PRIVATE tabulate::tabulate)

add_executable(nested_table_colors nested_table_colors.cpp)
target_link_libraries(nested_table_colors PRIVATE tabulate::tabulate)

add_executable(no_trim no_trim.cpp)
target_link_libraries(no_trim PRIVATE tabulate::tabulate)

add_executable(padding padding.cpp)
target_link_libraries(padding PRIVATE tabulate::tabulate)

add_executable(runic runic.cpp)
target_link_libraries(runic PRIVATE tabulate::tabulate)

add_executable(shape shape.cpp)
target_link_libraries(shape PRIVATE tabulate::tabulate)

add_executable(summary summary.cpp)
target_link_libraries(summary PRIVATE tabulate::tabulate)

add_executable(unicode unicode.cpp)
target_link_libraries(unicode PRIVATE tabulate::tabulate)

add_executable(universal_constants universal_constants.cpp)
target_link_libraries(universal_constants PRIVATE tabulate::tabulate)

add_executable(word_wrapping word_wrapping.cpp)
target_link_libraries(word_wrapping PRIVATE tabulate::tabulate)

add_executable(asciidoc_export asciidoc_export.cpp)
target_link_libraries(asciidoc_export PRIVATE tabulate::tabulate)

add_executable(refresh_table refresh_table.cpp) 
target_link_libraries(refresh_table PRIVATE tabulate::tabulate ${CMAKE_THREAD_LIBS_INIT})

add_executable(remove_column remove_column.cpp)
target_link_libraries(remove_column PRIVATE tabulate::tabulate)

add_executable(nested_table_hidden_border nested_table_hidden_border.cpp)
target_link_libraries(nested_table_hidden_border PRIVATE tabulate::tabulate)

add_executable(format_after_print format_after_print.cpp)
target_link_libraries(format_after_print PRIVATE tabulate::tabulate)

add_executable(preserve_indentation preserve_indentation.cpp)
target_link_libraries(preserve_indentation PRIVATE tabulate::tabulate)

add_executable(border_styles border_styles.cpp)
target_link_libraries(border_styles PRIVATE tabulate::tabulate)

add_executable(string_view_in_row string_view_in_row.cpp) 
target_link_libraries(string_view_in_row PRIVATE tabulate::tabulate ${CMAKE_THREAD_LIBS_INIT})
