CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)

# Ensure that we can find ElementsProjectConfig.cmake
# (this works only for projects embedding ElementsProjectConfig.cmake)
if(NOT ElementsProject_DIR AND ("$ENV{ElementsProject_DIR}" STREQUAL ""))
  set(ElementsProject_DIR ${CMAKE_SOURCE_DIR}/cmake)
endif()

#---------------------------------------------------------------
# Load macros and functions for Elements-based projects
find_package(ElementsProject)
#---------------------------------------------------------------

# Declare project name and version
elements_project(Elements 1.0)

# This test does not really fit in a subdirectory.
elements_add_test(cmake.EnvConfigTests
               COMMAND nosetests ${CMAKE_SOURCE_DIR}/cmake/EnvConfig)
