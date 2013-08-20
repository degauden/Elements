#
# FindCCfits.cmake
#
# Created on: Aug 19, 2013
#     Author: Pavel Binko
#
# -- Check for CCfits library --
#
# Defines:
#
#  CCFITS_FOUND
#  CCFITS_INCLUDE
#  CCFITS_LIBRARY
#
# Hints for non-standard locations:
#
#   CCFITS_ROOT_DIR (used for finding CCfits)
#
# Source of information :
#
#   https://github.com/wangd/SciDB-HDF5/blob/master/cmake/Modules/FindCfitsio.cmake
#

include(FindPackageHandleStandardArgs)

# Look for includes and libraries

find_path(CCFITS_INCLUDE_DIR CCfits/CCfits
  HINTS ${CCFITS_ROOT_DIR}
  PATH_SUFFIXES include)

find_library(CCFITS_LIBRARY CCfits
  HINTS ${CCFITS_ROOT_DIR}
  PATH_SUFFIXES lib)

find_package_handle_standard_args(CCFITS DEFAULT_MSG
  CCFITS_INCLUDE_DIR CCFITS_LIBRARY)

if(NOT CCFITS_FOUND)
  message("Can't find CCfits. You can specify a non-standard (e.g., from source) installation using CCFITS_ROOT_DIR")
endif(NOT CCFITS_FOUND)


mark_as_advanced(CCFITS_LIBRARY CCFITS_INCLUDE_DIR)
