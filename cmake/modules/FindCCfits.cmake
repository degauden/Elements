# -*- cmake -*-
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
if(NOT CCFITS_FOUND)

  find_path(CCFITS_INCLUDE_DIR CCfits/CCfits)

  find_library(CCFITS_LIBRARIES NAMES CCfits)

  set(CCFITS_INCLUDE_DIRS ${CCFITS_INCLUDE_DIR})

# handle the QUIETLY and REQUIRED arguments and set CCFITS_FOUND to TRUE if
# all listed variables are TRUE
  INCLUDE(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(CCfits DEFAULT_MSG CCFITS_INCLUDE_DIR CCFITS_LIBRARIES)

  mark_as_advanced(CCFITS_FOUND CCFITS_INCLUDE_DIR CCFITS_LIBRARIES)

endif()
