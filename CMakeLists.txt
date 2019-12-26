cmake_minimum_required(VERSION 3.8)
project(tabulate VERSION 1.0.0 LANGUAGES CXX)
option(tabulate_BUILD_TESTS OFF)
option(SAMPLES "Build Samples" OFF)

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

add_library(tabulate INTERFACE)
add_library(tabulate::tabulate ALIAS tabulate)

target_compile_features(tabulate INTERFACE cxx_std_17)
target_include_directories(tabulate INTERFACE
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

if( SAMPLES )
  add_subdirectory(samples)
endif()

configure_package_config_file(tabulateConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/tabulateConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tabulate)

install(TARGETS tabulate EXPORT tabulateTargets)
install(EXPORT tabulateTargets
        FILE tabulateTargets.cmake
        NAMESPACE tabulate::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tabulate)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tabulateConfig.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tabulate)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/tabulate
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        USE_SOURCE_PERMISSIONS
        PATTERN "*.hpp")
