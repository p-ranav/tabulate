add_executable(employees employees.cpp)
target_link_libraries(employees PRIVATE tabulate::tabulate)

add_executable(mario mario.cpp)
target_link_libraries(mario PRIVATE tabulate::tabulate)

add_executable(movies movies.cpp)
target_link_libraries(movies PRIVATE tabulate::tabulate)

add_executable(font_styles font_styles.cpp)
target_link_libraries(font_styles PRIVATE tabulate::tabulate)

add_executable(summary summary.cpp)
target_link_libraries(summary PRIVATE tabulate::tabulate)

add_executable(universal_constants universal_constants.cpp)
target_link_libraries(universal_constants PRIVATE tabulate::tabulate)