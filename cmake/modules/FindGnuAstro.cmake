# -*- cmake -*-
#
# FindGnuAstro.cmake
#
# Created on: Aug 19, 2013
#     Author: Pavel Binko
#
# -- Check for GnuAstro library --
#
# Defines:
#
#  GNUASTRO_FOUND
#  GNUASTRO_INCLUDE
#  GNUASTRO_LIBRARY
#
# Hints for non-standard locations:
#
#   GNUASTRO_ROOT_DIR (used for finding GnuAstro)
#
# Source of information :
#
#   https://github.com/wangd/SciDB-HDF5/blob/master/cmake/Modules/FindCfitsio.cmake
#
if(NOT GNUASTRO_FOUND)

  find_path(GNUASTRO_INCLUDE_DIR gnuastro/fits.h
            HINTS ENV GNUASTRO_INSTALL_DIR
            PATH_SUFFIXES include)

  find_library(GNUASTRO_LIBRARY gnuastro
               HINTS ENV GNUASTRO_INSTALL_DIR
               PATH_SUFFIXES lib)


  set(GNUASTRO_LIBRARIES ${GNUASTRO_LIBRARY})
  set(GNUASTRO_INCLUDE_DIRS ${GNUASTRO_INCLUDE_DIR})

# handle the QUIETLY and REQUIRED arguments and set GNUASTRO_FOUND to TRUE if
# all listed variables are TRUE
  INCLUDE(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(GnuAstro DEFAULT_MSG GNUASTRO_INCLUDE_DIRS GNUASTRO_LIBRARIES)

  mark_as_advanced(GNUASTRO_FOUND GNUASTRO_INCLUDE_DIRS GNUASTRO_LIBRARIES)

  find_package(Cfitsio)

  set(GNUASTRO_LIBRARIES ${GNUASTRO_LIBRARIES} ${CFITSIO_LIBRARIES})
  set(GNUASTRO_INCLUDE_DIRS ${GNUASTRO_INCLUDE_DIRS} ${CFITSIO_INCLUDE_DIRS})

  find_package(WCSLIB)

  set(GNUASTRO_LIBRARIES ${GNUASTRO_LIBRARIES} ${WCSLIB_LIBRARIES})
  set(GNUASTRO_INCLUDE_DIRS ${GNUASTRO_INCLUDE_DIRS} ${WCSLIB_INCLUDE_DIRS})

  find_package(GSL)

  set(GNUASTRO_LIBRARIES ${GNUASTRO_LIBRARIES} ${GSL_LIBRARIES})
  set(GNUASTRO_INCLUDE_DIRS ${GNUASTRO_INCLUDE_DIRS} ${GSL_INCLUDE_DIRS})

  list(REMOVE_DUPLICATES GNUASTRO_LIBRARIES)
  list(REMOVE_DUPLICATES GNUASTRO_INCLUDE_DIRS)

endif()
