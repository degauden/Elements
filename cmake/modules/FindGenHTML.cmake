if (NOT GENHTML_FOUND)

    find_package(LCov)

    if(NOT LCOV_EXECUTABLE)
        message(WARNING "lcov not found! Aborting...")
    endif() # no LCOV_EXECUTABLE

    find_program(GENHTML_EXECUTABLE NAMES genhtml
                 HINTS ENV GENHTML_INSTALL_DIR LCOV_INSTALL_DIR GCOV_INSTALL_DIR
                 PATH_SUFFIXES bin)
    set(GENHTML_EXECUTABLE ${GENHTML_EXECUTABLE} CACHE STRING "")

# handle the QUIETLY and REQUIRED arguments and set GENHTML_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(GenHTML DEFAULT_MSG GENHTML_EXECUTABLE)

    mark_as_advanced(GENHTML_FOUND GENHTML_EXECUTABLE)


endif (NOT GENHTML_FOUND)
