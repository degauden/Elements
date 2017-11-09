if (NOT VALGRIND_FOUND)

    find_program(VALGRIND_EXECUTABLE valgrind
                 HINTS ENV VALGRIND_INSTALL_DIR
                 PATH_SUFFIXES bin)
    set(VALGRIND_EXECUTABLE ${VALGRIND_EXECUTABLE} CACHE STRING "")

# handle the QUIETLY and REQUIRED arguments and set VALGRIND_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(Valgrind DEFAULT_MSG VALGRIND_EXECUTABLE)

    mark_as_advanced(VALGRIND_FOUND VALGRIND_EXECUTABLE)


endif (NOT VALGRIND_FOUND)

