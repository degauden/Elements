# -*- cmake -*-
# - Locate CppUnit library
# Defines:
#
#  CPPUNIT_FOUND
#  CPPUNIT_INCLUDE_DIR
#  CPPUNIT_INCLUDE_DIRS (not cached)
#  CPPUNIT_LIBRARIES

if(NOT CPPUNIT_FOUND)

  find_path(CPPUNIT_INCLUDE_DIR cppunit/Test.h
            HINTS ENV CPPUNIT_INSTALL_DIR
            PATH_SUFFIXES include)
  find_library(CPPUNIT_LIBRARY cppunit
               HINTS ENV CPPUNIT_INSTALL_DIR
               PATH_SUFFIXES lib)

  set(CPPUNIT_LIBRARIES ${CPPUNIT_LIBRARY})
  set(CPPUNIT_INCLUDE_DIRS ${CPPUNIT_INCLUDE_DIR})


# handle the QUIETLY and REQUIRED arguments and set CPPUNIT_FOUND to TRUE if
# all listed variables are TRUE
  INCLUDE(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(CppUnit DEFAULT_MSG CPPUNIT_INCLUDE_DIRS CPPUNIT_LIBRARIES)

  mark_as_advanced(CPPUNIT_FOUND CPPUNIT_INCLUDE_DIRS CPPUNIT_LIBRARIES)

endif()