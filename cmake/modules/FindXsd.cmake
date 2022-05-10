# -*- cmake -*-
# - Locate Xsd library
# Defines:
#
#  XSD_FOUND
#  XSD_INCLUDE_DIR
#  XSD_INCLUDE_DIRS (not cached)
#
# Imports:
#
#  Xsd::xsd
#

# Find quietly if already found before
if(DEFINED CACHE{XSD_INCLUDE_DIR})
  set(${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY YES)
endif()

find_program(XSDCXX_EXECUTABLE
             NAMES xsdcxx xsd
             HINTS ENV XSD_ROOT_DIR XSD_INSTALL_DIR
             PATH_SUFFIXES bin)

find_path(XSD_INCLUDE_DIR xsd/cxx/config.hxx
          HINTS ENV XSD_ROOT_DIR XSD_INSTALL_DIR
          PATH_SUFFIXES include)


# handle the QUIETLY and REQUIRED arguments and set XSD_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Xsd DEFAULT_MSG XSDCXX_EXECUTABLE XSD_INCLUDE_DIR)
mark_as_advanced(XSD_FOUND XSDCXX_EXECUTABLE XSD_INCLUDE_DIR)

set(XSD_INCLUDE_DIRS ${XSD_INCLUDE_DIR})

# Modernisation: create an interface target to link against
if(TARGET Xsd::xsd)
    return()
endif()
if(XSD_FOUND)
  add_library(Xsd::xsd IMPORTED INTERFACE)
  target_include_directories(Xsd::xsd SYSTEM INTERFACE "${XSD_INCLUDE_DIRS}")
  # Display the imported target for the user to know
  if(NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
    message(STATUS "  Import target: Xsd::xsd")
  endif()
endif()
