if (NOT PYTEST_FOUND)

	find_program(PYTEST_EXECUTABLE py.test)
	set(PYTEST_EXECUTABLE ${PYTEST_EXECUTABLE} CACHE STRING "")

# handle the QUIETLY and REQUIRED arguments and set PYTEST_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(PyTest DEFAULT_MSG PYTEST_EXECUTABLE)

    mark_as_advanced(NOSE_FOUND PYTEST_EXECUTABLE)


endif (NOT PYTEST_FOUND)

