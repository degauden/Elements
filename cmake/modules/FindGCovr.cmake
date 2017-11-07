if (NOT GCOVR_FOUND)

    find_package(GCov)

    if(NOT GCOV_EXECUTABLE)
        message(WARNING "gcov not found! Aborting...")
    endif() # no GCOV_EXECUTABLE

	find_program(GCOVR_EXECUTABLE NAMES gcovr
	             HINTS ENV GCOVR_INSTALL_DIR GCOV_INSTALL_DIR
	             PATH_SUFFIXES bin)
	set(GCOVR_EXECUTABLE ${GCOVR_EXECUTABLE} CACHE STRING "")

# handle the QUIETLY and REQUIRED arguments and set GCOVR_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(GCovr DEFAULT_MSG GCOVR_EXECUTABLE)

    mark_as_advanced(GCOVR_FOUND GCOVR_EXECUTABLE)


endif (NOT GCOVR_FOUND)

