if(NOT FITSVERIFY_FOUND)

  find_program(FITSVERIFY_PROGRAM fitsverify
               HINTS ENV FITSVERIFY_ROOT_DIR
               PATH_SUFFIXES bin)


  set(FITSVERIFY_PROGRAMS ${FITSVERIFY_PROGRAM})

# handle the QUIETLY and REQUIRED arguments and set FITSVERIFY_FOUND to TRUE if
# all listed variables are TRUE
  INCLUDE(FindPackageHandleStandardArgs)
  FIND_PACKAGE_HANDLE_STANDARD_ARGS(Fitsverify DEFAULT_MSG FITSVERIFY_PROGRAMS)

  mark_as_advanced(FITSVERIFY_FOUND FITSVERIFY_PROGRAMS)

  list(REMOVE_DUPLICATES FITSVERIFY_PROGRAMS)

endif()
