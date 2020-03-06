if (NOT PLANTUML_FOUND)

    find_program(PLANTUML_EXECUTABLE plantuml
                 HINTS ENV PLANTUML_INSTALL_DIR
                 PATH_SUFFIXES bin)
    set(PLANTUML_EXECUTABLE ${PLANTUML_EXECUTABLE} CACHE STRING "")


    find_package(Java QUIET)

    if(Java_FOUND)

      include(UseJava)

      find_jar(PLANTUML_JARFILE
               NAMES plantuml
               HINTS ENV PLANTUML_INSTALL_DIR)

    endif()

# handle the QUIETLY and REQUIRED arguments and set PLANTUML_FOUND to TRUE if
# all listed variables are TRUE
    INCLUDE(FindPackageHandleStandardArgs)
    FIND_PACKAGE_HANDLE_STANDARD_ARGS(PlantUML DEFAULT_MSG PLANTUML_EXECUTABLE PLANTUML_JARFILE)

    mark_as_advanced(PLANTUML_FOUND PLANTUML_EXECUTABLE PLANTUML_JARFILE)


endif (NOT PLANTUML_FOUND)

