if(NOT GTEST_FOUND)

find_path(GTEST_INCLUDE_DIR gtest/gtest.h
          HINTS $ENV{GTEST_INSTALL_DIR}/include )

find_library(GTEST_LIBRARY NAMES gtest gtest_main
             HINTS $ENV{GTEST_INSTALL_DIR} )

set(GTEST_LIBRARIES ${GTEST_LIBRARY} )
set(GTEST_INCLUDE_DIRS ${GTEST_INCLUDE_DIR} )

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LIBXML2_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(GTest  DEFAULT_MSG
                                  GTEST_LIBRARY GTEST_INCLUDE_DIR)

mark_as_advanced(GTEST_FOUND GTEST_INCLUDE_DIR GTEST_LIBRARY )

endif()

