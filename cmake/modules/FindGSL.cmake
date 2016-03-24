# - Locate GSL library
# Defines:
#
#  GSL_FOUND
#  GSL_INCLUDE_DIR
#  GSL_INCLUDE_DIRS (not cached)
#  GSL_LIBRARY
#  GSL_CBLAS_LIBRARY
#  GSL_LIBRARIES (not cached)


if(NOT GSL_FOUND)

  find_path(GSL_INCLUDE_DIR gsl/gsl_version.h
            HINTS ENV GSL_ROOT_DIR
            PATH_SUFFIXES include)

  find_library(GSL_LIBRARY gsl
               HINTS ENV GSL_ROOT_DIR
               PATH_SUFFIXES lib)

  find_library(GSL_CBLAS_LIBRARY gslcblas
               HINTS ENV GSL_ROOT_DIR
               PATH_SUFFIXES lib)


  set(GSL_LIBRARIES ${GSL_LIBRARY} ${GSL_CBLAS_LIBRARY})
  set(GSL_INCLUDE_DIRS ${GSL_INCLUDE_DIR})

# handle the QUIETLY and REQUIRED arguments and set CCFITS_FOUND to TRUE if
# all listed variables are TRUE
  INCLUDE(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(GSL DEFAULT_MSG GSL_INCLUDE_DIRS GSL_LIBRARIES)

  mark_as_advanced(GSL_FOUND GSL_INCLUDE_DIRS GSL_LIBRARIES)

  list(REMOVE_DUPLICATES GSL_LIBRARIES)
  list(REMOVE_DUPLICATES GSL_INCLUDE_DIRS)

endif()
