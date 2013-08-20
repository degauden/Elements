#
# FindCfitsio.cmake
#
# Created on: Aug 19, 2013
#     Author: Pavel Binko
#
# -- Check for CFITSIO library --
#
# Defines:
#
#  CFITSIO_FOUND - system has the CFITSIO library
#  CFITSIO_INCLUDE_DIR - the CFITSIO include directory
#  CFITSIO_LIBRARY - The library needed to use CFITSIO
#
# Hints for non-standard locations:
#
#   CFITSIO_ROOT_DIR (used for finding cfitsio)
#
# Source of information :
#
#   https://github.com/wangd/SciDB-HDF5/blob/master/cmake/Modules/FindCfitsio.cmake
#

include(FindPackageHandleStandardArgs)

# Look for includes and libraries

find_path(CFITSIO_INCLUDE_DIR
  NAMES fitsio.h
  HINTS ${CFITSIO_ROOT_DIR}
  PATH_SUFFIXES include include/cfitsio)

find_library(CFITSIO_LIBRARY cfitsio
  HINTS ${CFITSIO_ROOT_DIR}
  PATH_SUFFIXES lib)

find_package_handle_standard_args(CFITSIO DEFAULT_MSG
  CFITSIO_INCLUDE_DIR CFITSIO_LIBRARY)

if(NOT CFITSIO_FOUND)
  message("Can't find cfitsio. You can specify a non-standard (e.g., from source) installation using CFITSIO_ROOT_DIR")
endif(NOT CFITSIO_FOUND)


mark_as_advanced(CFITSIO_LIBRARY CFITSIO_INCLUDE_DIR)
