if (NOT GCOV_FOUND)

	find_program(GCOV_EXECUTABLE NAMES gcov
	             HINTS ENV GCOV_INSTALL_DIR
	             PATH_SUFFIXES bin)
	set(GCOV_EXECUTABLE ${GCOV_EXECUTABLE} CACHE STRING "")

# handle the QUIETLY and REQUIRED arguments and set GCOV_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(GCov DEFAULT_MSG GCOV_EXECUTABLE)

    mark_as_advanced(GCOV_FOUND GCOV_EXECUTABLE)


endif (NOT GCOV_FOUND)

