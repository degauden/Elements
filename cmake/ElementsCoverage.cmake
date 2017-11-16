include_guard()


  add_custom_target(cov
                    COMMENT "Generating the coverage report" VERBATIM)

if(CMAKE_BUILD_TYPE STREQUAL Coverage)

  find_package(GenHTML)

  if(GENHTML_EXECUTABLE AND LCOV_EXECUTABLE)

    add_custom_target(lcov_init ALL
                    COMMAND ${LCOV_EXECUTABLE} --zerocounters --directory ${PROJECT_BINARY_DIR}
                    COMMAND COMMAND ${LCOV_EXECUTABLE} --directory ${PROJECT_BINARY_DIR} --initial --capture --output-file ${PROJECT_NAME}.info
                    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/cov/lcov
                    COMMENT "Initialize the coverage info" VERBATIM)
    add_dependencies(lcov_init lcov_dir)

  add_custom_command(TARGET lcov_init POST_BUILD
                    COMMAND ;
                    COMMENT "Please run:make; make test; make cov in order to get the coverage reports"
                    )


    add_custom_target(lcov
                    COMMAND ${LCOV_EXECUTABLE} --directory ${PROJECT_BINARY_DIR} --capture --output-file ${PROJECT_NAME}.info
                    COMMAND ${LCOV_EXECUTABLE} --remove ${PROJECT_NAME}.info /usr/include/* */InstallArea/* ${BUILD_SUBDIR}/* ${PROJECT_BINARY_DIR}/* --output-file ${PROJECT_NAME}.info.cleaned
                    COMMAND ${GENHTML_EXECUTABLE} -o html ${PROJECT_NAME}.info.cleaned
                    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/cov/lcov
                    COMMENT "Resetting code coverage counters to zero.\nProcessing code coverage counters" VERBATIM)

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

  find_package(GCovr)

    if(GCOVR_EXECUTABLE)

      add_custom_target(gcovr
                        COMMAND ${GCOVR_EXECUTABLE} -x -r ${CMAKE_SOURCE_DIR} --exclude=/usr/include/.* --exclude=${PROJECT_BINARY_DIR}/.* --exclude=.*/InstallArea/.* -o ${PROJECT_NAME}.xml
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
          PATH_SUFFIXES auxdir
          NO_DEFAULT_PATH)
          
add_custom_command(TARGET cov POST_BUILD
                    COMMAND ${ctest2junit_cmd} ${PROJECT_BINARY_DIR} ${ctest2junit_xsl_file}
                    )

endif()