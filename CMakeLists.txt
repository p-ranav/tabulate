cmake_minimum_required(VERSION 3.8)
project(tabulate VERSION 1.3.0 LANGUAGES CXX)
option(tabulate_BUILD_TESTS OFF)
option(SAMPLES "Build Samples" OFF)

if(MSVC)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")
endif()

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

add_library(tabulate INTERFACE)
add_library(tabulate::tabulate ALIAS tabulate)

if (USE_CPP17)
  target_compile_features(tabulate INTERFACE cxx_std_17)
  message(STATUS "Using C++17")
else()
  target_compile_features(tabulate INTERFACE cxx_std_11)
  message(STATUS "Using C++11")
endif()

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
