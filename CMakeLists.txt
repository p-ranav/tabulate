cmake_minimum_required(VERSION 3.10)
project(tabulate VERSION 2.2 LANGUAGES CXX)
option(tabulate_BUILD_TESTS "Build Tests" OFF)
option(tabulate_BUILD_MODULES "Build C++20 Module" OFF)
option(SAMPLES "Build Samples" OFF)

set(TABULATE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Versioning
# ===========

# Project version
file(STRINGS "${TABULATE_INCLUDE_DIR}/tabulate/tabulate.hpp" tabulate_version_defines
     REGEX "#define TABULATE_VERSION_(MAJOR|MINOR|PATCH)")
foreach(ver ${tabulate_version_defines})
    if(ver MATCHES "#define TABULATE_VERSION_(MAJOR|MINOR|PATCH) +([^ ]+)$")
        set(TABULATE_VERSION_${CMAKE_MATCH_1} "${CMAKE_MATCH_2}" CACHE INTERNAL "")
    endif()
endforeach()
set(TABULATE_VERSION
    ${TABULATE_VERSION_MAJOR}.${TABULATE_VERSION_MINOR}.${TABULATE_VERSION_PATCH})
message(STATUS "tabulate version: v${TABULATE_VERSION}")

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

if (USE_CPP20)
  target_compile_features(tabulate INTERFACE cxx_std_20)
  message(STATUS "Using C++20")
elseif (USE_CPP17)
  target_compile_features(tabulate INTERFACE cxx_std_17)
  message(STATUS "Using C++17")
else()
  target_compile_features(tabulate INTERFACE cxx_std_11)
  message(STATUS "Using C++11")
endif()

target_include_directories(tabulate INTERFACE
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)

if(tabulate_BUILD_MODULES)
  if(CMAKE_VERSION VERSION_LESS "3.28")
    message(FATAL_ERROR "tabulate_BUILD_MODULES requires CMake 3.28 or newer (found ${CMAKE_VERSION})")
  endif()

  message(STATUS "Building C++20 module")
  add_library(tabulate_module)
  add_library(tabulate::tabulate_module ALIAS tabulate_module)
  target_sources(tabulate_module
    PUBLIC
      FILE_SET CXX_MODULES
      BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/module
      FILES ${CMAKE_CURRENT_SOURCE_DIR}/module/tabulate.cppm)
  target_compile_features(tabulate_module PUBLIC cxx_std_20)
  target_link_libraries(tabulate_module PRIVATE tabulate::tabulate)
  set_target_properties(tabulate_module PROPERTIES CXX_SCAN_FOR_MODULES ON)
endif()

if( SAMPLES )
  add_subdirectory(samples)
endif()

if( tabulate_BUILD_TESTS )
  enable_testing()
  add_subdirectory(test)
endif()

configure_package_config_file(tabulateConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/tabulateConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tabulate)

write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
                                 VERSION ${TABULATE_VERSION}
                                 COMPATIBILITY AnyNewerVersion)

if(tabulate_BUILD_MODULES)
  install(TARGETS tabulate tabulate_module
          EXPORT tabulateTargets
          FILE_SET CXX_MODULES DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tabulate/modules)
  install(EXPORT tabulateTargets
          FILE tabulateTargets.cmake
          NAMESPACE tabulate::
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tabulate
          CXX_MODULES_DIRECTORY cxx-modules)
else()
  install(TARGETS tabulate EXPORT tabulateTargets)
  install(EXPORT tabulateTargets
          FILE tabulateTargets.cmake
          NAMESPACE tabulate::
          DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tabulate)
endif()
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/tabulateConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/tabulate)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/tabulate
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        USE_SOURCE_PERMISSIONS
        PATTERN "*.hpp")
