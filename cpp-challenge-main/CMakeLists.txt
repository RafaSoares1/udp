cmake_minimum_required(VERSION 3.16)

project(bisect-cpp-challenge LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(StandardProjectSettings)
include(PreventInSourceBuilds)
include(CompilerWarnings)

# Link this 'library' to set the c++ standard / compile-time options requested
add_library(project_options INTERFACE)
target_compile_features(project_options INTERFACE cxx_std_20)
add_library(bisect::project_options ALIAS project_options)

# Link this 'library' to use the warnings specified in CompilerWarnings.cmake
add_library(project_warnings INTERFACE)
add_library(bisect::project_warnings ALIAS project_warnings)

# Standard compiler warnings
include(CompilerWarnings)
set_project_warnings(project_warnings FALSE)

add_subdirectory(demo)
add_subdirectory(lib)
