if (NOT CCACHE_FOUND)

	find_program(CCACHE_EXECUTABLE NAMES ccache ccache-swig)
	set(CCACHE_EXECUTABLE ${CCACHE_EXECUTABLE} CACHE STRING "")

# handle the QUIETLY and REQUIRED arguments and set CCACHE_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(CCache DEFAULT_MSG CCACHE_EXECUTABLE)

    mark_as_advanced(CCACHE_FOUND CCACHE_EXECUTABLE)


endif (NOT CCACHE_FOUND)

