CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)

# Ensure that we can find ElementsProjectConfig.cmake
# (this works only for projects embedding ElementsProjectConfig.cmake)
if(NOT ElementsProject_DIR AND ("$ENV{ElementsProject_DIR}" STREQUAL ""))
  set(ElementsProject_DIR ${CMAKE_SOURCE_DIR}/cmake)
endif()

#---------------------------------------------------------------
# Load macros and functions for Elements-based projects. The find_package
# function detects the ElementsProjectConfig.cmake file.
find_package(ElementsProject)
#---------------------------------------------------------------

# Declare project name and version
set(ELEMENTS_HIDE_SYMBOLS ON
    CACHE STRING "enable explicit symbol visibility on gcc-4"
    FORCE)


elements_project(Elements 3.10 DESCRIPTION "A C++ base framework for the Euclid Software.")


# This test does not really fit in a subdirectory.

add_python_test_dir(cmake/scripts/EnvConfig PREFIX cmake.EnvConfig PATTERN "Test*.py")
