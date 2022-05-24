#[=======================================================================[.rst:
ElementsCoverage
-----------------

This files implements the functions for the production of code instrumented
for coverage

#]=======================================================================]


include_guard(GLOBAL)


  add_custom_target(cov
                    COMMENT "Generating the coverage report" VERBATIM)


if("${CMAKE_BUILD_TYPE}" STREQUAL "Coverage")

  find_package(GenHTML QUIET)

  if(GENHTML_EXECUTABLE AND LCOV_EXECUTABLE)

    add_custom_target(lcov_init ALL
                    COMMAND ${LCOV_EXECUTABLE} --zerocounters --directory ${PROJECT_BINARY_DIR}
                    COMMAND ${LCOV_EXECUTABLE} --directory ${PROJECT_BINARY_DIR} --initial --capture --output-file ${PROJECT_NAME}.info || exit "No gcno files"
                    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/cov/lcov
                    COMMENT "Initialize the coverage info" VERBATIM)

    add_dependencies(lcov_init lcov_dir)

  add_custom_command(TARGET lcov_init POST_BUILD
                    COMMAND ;
                    COMMENT "Please run:make; make test; make cov in order to get the coverage reports"
                    )
   if(NOT SQUEEZED_INSTALL)
    add_custom_target(lcov
                      COMMAND ${LCOV_EXECUTABLE} --directory ${PROJECT_BINARY_DIR} --capture --output-file ${PROJECT_NAME}.info
                      COMMAND ${LCOV_EXECUTABLE} --remove ${PROJECT_NAME}.info /usr/include/* ${ELEMENTS_BASE_PREFIX_DIR}/include/* ${ELEMENTS_BASE_PREFIX_DIR}/usr/include/* ${ELEMENTS_BASE_PREFIX_DIR}/lib/gcc/* ${ELEMENTS_BASE_PREFIX_DIR}/x86_64-conda* */InstallArea/* ${BUILD_SUBDIR}/* ${PROJECT_BINARY_DIR}/* /usr/lib/gcc/* --output-file ${PROJECT_NAME}.info.cleaned
                      COMMAND ${GENHTML_EXECUTABLE} -o html ${PROJECT_NAME}.info.cleaned
                      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/cov/lcov
                      COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters" VERBATIM)
    else()
    add_custom_target(lcov
                      COMMAND ${LCOV_EXECUTABLE} --directory ${PROJECT_BINARY_DIR} --capture --output-file ${PROJECT_NAME}.info
                      COMMAND ${LCOV_EXECUTABLE} --remove ${PROJECT_NAME}.info /usr/include/* */InstallArea/* ${BUILD_SUBDIR}/* ${PROJECT_BINARY_DIR}/* /usr/lib/gcc/* --output-file ${PROJECT_NAME}.info.cleaned
                      COMMAND ${GENHTML_EXECUTABLE} -o html ${PROJECT_NAME}.info.cleaned
                      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/cov/lcov
                      COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters" VERBATIM)
    endif()


    add_dependencies(cov lcov)

    add_custom_target(lcov_dir
                      COMMAND  ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/cov/lcov
                      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                      COMMENT "Create the lcov output directory" VERBATIM)
  
    add_dependencies(lcov lcov_dir)

    add_custom_command(TARGET lcov POST_BUILD
                      COMMAND ;
                      COMMENT "===================================================================================================\nOpen ./${BUILD_SUBDIR}/cov/lcov/html/index.html in your browser to view the coverage report.\n===================================================================================================\n"
                      )

    find_package(GCovr QUIET)

    if(GCOVR_EXECUTABLE)
    
      set(GCOVR_OPTIONS)
      set(GCOVR_OPTIONS ${GCOVR_OPTIONS} --exclude="/usr/include/.*")
      set(GCOVR_OPTIONS ${GCOVR_OPTIONS} --exclude="${PROJECT_BINARY_DIR}/.*")
      if(NOT SQUEEZED_INSTALL)
        set(GCOVR_OPTIONS ${GCOVR_OPTIONS} --exclude="${ELEMENTS_BASE_PREFIX_DIR}/include/.*")
        set(GCOVR_OPTIONS ${GCOVR_OPTIONS} --exclude="${ELEMENTS_BASE_PREFIX_DIR}/usr/include/.*")
        set(GCOVR_OPTIONS ${GCOVR_OPTIONS} --exclude="${ELEMENTS_BASE_PREFIX_DIR}/lib/gcc/.*")
        set(GCOVR_OPTIONS ${GCOVR_OPTIONS} --exclude="${ELEMENTS_BASE_PREFIX_DIR}/x86_64-conda.*")
      endif()
      set(GCOVR_OPTIONS ${GCOVR_OPTIONS} --exclude=".*/InstallArea/.*")        
      
      if(GCOVR_EXCLUDE_UNREACHABLE)
        set(GCOVR_OPTIONS ${GCOVR_OPTIONS} --exclude-unreachable-branches)
      endif()
    
      if(GCOVR_EXCLUDE_THROW)
        set(GCOVR_OPTIONS ${GCOVR_OPTIONS} --exclude-throw-branches)
      endif()

      set(GCOVR_OPTIONS ${GCOVR_OPTIONS} ${GCOVR_EXTRA_OPTIONS})        

      add_custom_target(gcovr
                        COMMAND ${GCOVR_EXECUTABLE} -x -r ${CMAKE_SOURCE_DIR} ${GCOVR_OPTIONS} -o ${PROJECT_NAME}.xml
                        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/cov/gcovr
                        COMMENT "Produce Cobertura output" VERBATIM)

      add_dependencies(cov gcovr)
    
      add_dependencies(lcov gcovr)

      add_custom_target(gcovr_dir
                        COMMAND  ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/cov/gcovr
                        WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
                        COMMENT "Create the gcovr output directory" VERBATIM)
  
      add_dependencies(gcovr gcovr_dir)

      add_custom_command(TARGET gcovr POST_BUILD
                        COMMAND ;
                        COMMENT "===================================================================================================\nThe ./${BUILD_SUBDIR}/cov/gcovr/${PROJECT_NAME}.xml file contains the Cobertura XML report.\n===================================================================================================\n"
                        )

     endif()
   endif()
   
   # The pytest directory has to be created at configure time
   find_python_module(pytest_cov)
   if (PY_PYTEST_COV AND "${PYFRMK_NAME}" STREQUAL "PyTest")
     file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/cov/${PYFRMK_NAME})
   endif()

else()

  add_custom_command(TARGET cov POST_BUILD
                     COMMAND ;
                     COMMENT "=======================================================================\nPlease build in coverage mode in order to instrument the binaries.\n=======================================================================\n"
                    )

endif()


find_python_module(lxml)

if(PY_LXML)
find_file(ctest2junit_xsl_file
          NAMES CTest2JUnit.xsl
          PATHS ${CMAKE_MODULE_PATH}
          PATH_SUFFIXES auxdir/test auxdir
          NO_DEFAULT_PATH)
          
add_custom_command(TARGET cov POST_BUILD
                   COMMAND ${ctest2junit_cmd} ${PROJECT_BINARY_DIR} ${ctest2junit_xsl_file}
                   )

endif()