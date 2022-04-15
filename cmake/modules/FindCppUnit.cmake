# -*- cmake -*-
# - Locate CppUnit library
# Defines:
#
#  CPPUNIT_FOUND
#  CPPUNIT_INCLUDE_DIR
#  CPPUNIT_INCLUDE_DIRS (not cached)
#  CPPUNIT_LIBRARY
#  CPPUNIT_LIBRARIES (not cached)
#
# Imports:
#
#  CppUnit::cppunit
#

# Find quietly if already found before
if(DEFINED CACHE{CPPUNIT_INCLUDE_DIR})
  set(${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY YES)
endif()



find_path(CPPUNIT_INCLUDE_DIR cppunit/Test.h
          HINTS ENV CPPUNIT_INSTALL_DIR
          PATH_SUFFIXES include)
find_library(CPPUNIT_LIBRARY cppunit
             HINTS ENV CPPUNIT_INSTALL_DIR
             PATH_SUFFIXES lib)



# handle the QUIETLY and REQUIRED arguments and set CPPUNIT_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(CppUnit DEFAULT_MSG CPPUNIT_INCLUDE_DIR CPPUNIT_LIBRARY)
mark_as_advanced(CPPUNIT_FOUND CPPUNIT_INCLUDE_DIR CPPUNIT_LIBRARY)

set(CPPUNIT_LIBRARIES ${CPPUNIT_LIBRARY})
set(CPPUNIT_INCLUDE_DIRS ${CPPUNIT_INCLUDE_DIR})

# Modernisation: create an interface target to link against
if(TARGET CppUnit::cppunit)
    return()
endif()
if(CPPUNIT_FOUND)
  add_library(CppUnit::cppunit IMPORTED INTERFACE)
  target_include_directories(CppUnit::cppunit SYSTEM INTERFACE "${CPPUNIT_INCLUDE_DIRS}")
  target_link_libraries(CppUnit::cppunit INTERFACE "${CPPUNIT_LIBRARIES}")
  # Display the imported target for the user to know
  if(NOT ${CMAKE_FIND_PACKAGE_NAME}_FIND_QUIETLY)
    message(STATUS "  Import target: CppUnit::cppunit")
  endif()
endif()
