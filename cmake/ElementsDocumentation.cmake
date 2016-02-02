  # Add Doxygen generation
  find_package(Doxygen)
  if(DOXYGEN_FOUND)

    find_file(doxygen_file_template
              NAMES Doxyfile.in
              PATHS ${CMAKE_MODULE_PATH}
              PATH_SUFFIXES doc
              NO_DEFAULT_PATH)


    if(doxygen_file_template)
      configure_file(
        "${doxygen_file_template}"
        "${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile"
        @ONLY
      )
      message(STATUS "Generated Doxygen configuration file: ${PROJECT_BINARY_DIR}/doc/doxygen/Doxyfile")
      message(STATUS "From the Doxygen.in template file: ${doxygen_file_template}")

    endif()

    add_custom_target(doc
                      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen/Doxyfile
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen
                      COMMENT "Generating API documentation with Doxygen" VERBATIM)

  endif()
  
  
  
  find_package(Sphinx)
  if(SPHINX_FOUND)
  
    find_file(sphinx_conf_template
              NAMES Sphinx_conf.py.in
              PATHS ${CMAKE_MODULE_PATH}
              PATH_SUFFIXES doc
              NO_DEFAULT_PATH)


    if(sphinx_conf_template)
      configure_file(
        "${sphinx_conf_template}"
        "${PROJECT_BINARY_DIR}/doc/sphinx/conf.py"
        @ONLY
      )
      message(STATUS "Generated Sphinx configuration file: ${PROJECT_BINARY_DIR}/doc/sphinx/conf.py")
      message(STATUS "From the template file: ${sphinx_conf_template}")

    endif()
    
    if(DOXYGEN_FOUND)
        

      find_file(cpp_rst_template
                NAMES cpp_modules.rst.in
                PATHS ${CMAKE_MODULE_PATH}
                PATH_SUFFIXES doc
                NO_DEFAULT_PATH)


      if(cpp_rst_template)
        configure_file(
          "${cpp_rst_template}"
          "${PROJECT_BINARY_DIR}/doc/sphinx/cpp_modules.rst"
          @ONLY
        )
        message(STATUS "Generated C++ main rst file: ${PROJECT_BINARY_DIR}/doc/sphinx/cpp_modules.rst")
        message(STATUS "From the template file: ${cpp_rst_template}")

    endif()

    
    endif()
    
    get_property(proj_python_package_list GLOBAL PROPERTY PROJ_PYTHON_PACKAGE_LIST)
    
    foreach (_py_pack IN LISTS proj_python_package_list)
        get_filename_component(_py_pack_short ${_py_pack} NAME)
        add_custom_target(sphinx_apidoc_${_py_pack_short}
                          COMMAND  ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/doc/sphinx/${_py_pack_short}
                          COMMAND  ${SPHINX_APIDOC_EXECUTABLE} -f -o ${PROJECT_BINARY_DIR}/doc/sphinx/${_py_pack_short} ${_py_pack}
                          COMMENT "Generating Sphinx API documentation for ${_py_pack_short}" VERBATIM)

        add_dependencies(doc sphinx_apidoc_${_py_pack_short})
    endforeach()
    
  endif()
  