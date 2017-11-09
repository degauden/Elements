if (NOT LCOV_FOUND)

    find_package(GCov)

    if(NOT GCOV_EXECUTABLE)
        message(WARNING "gcov not found!")
    endif() # no GCOV_EXECUTABLE

	find_program(LCOV_EXECUTABLE NAMES lcov
	             HINTS ENV LCOV_INSTALL_DIR GCOV_INSTALL_DIR
	             PATH_SUFFIXES bin)
	set(LCOV_EXECUTABLE ${LCOV_EXECUTABLE} CACHE STRING "")

# handle the QUIETLY and REQUIRED arguments and set LCOV_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(LCov DEFAULT_MSG LCOV_EXECUTABLE)

    mark_as_advanced(LCOV_FOUND LCOV_EXECUTABLE)


endif (NOT LCOV_FOUND)

