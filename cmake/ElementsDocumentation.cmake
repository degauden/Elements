  # Add Doxygen generation
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    
    # Generation of the main Doxygen configuration: the Doxyfile
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

    # add the doxygen target
    add_custom_target(doxygen
                      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen/Doxyfile
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen
                      COMMENT "Generating API documentation with Doxygen" VERBATIM)

  endif()
  
  
  
  find_package(Sphinx)
  if(SPHINX_FOUND)
    
    # Generation of the main sphinx configuration file.
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
    
    file(GLOB rst-files ${CMAKE_CURRENT_SOURCE_DIR}/doc/*.rst)
        
    foreach(r_file ${rst-files})
      get_filename_component(r_file_short ${r_file} NAME)      
      get_filename_component(r_file_short_we ${r_file} NAME_WE)
      if(NOT r_file_short_we STREQUAL "index")      
        configure_file(
          "${r_file}"
          "${PROJECT_BINARY_DIR}/doc/sphinx/${r_file_short}"
          COPYONLY
        )    
        message(STATUS "Copy ${r_file} file to ${PROJECT_BINARY_DIR}/doc/sphinx/${r_file_short}")
        if(NOT SPHINX_PROJECT_RELATED_PAGES)
          set(SPHINX_PROJECT_RELATED_PAGES "
Related Pages
=============

.. toctree::
   :maxdepth: 2
")  
        endif()
        set(SPHINX_PROJECT_RELATED_PAGES "${SPHINX_PROJECT_RELATED_PAGES}
   ${r_file_short_we}")
      endif()  
    endforeach()
    
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
        set(SPHINX_CPP_MODULES "cpp_modules")

    endif()

    
    endif()
    
    set(SPHINX_APIDOC_CMD ${env_cmd} --xml ${env_xml} ${SPHINX_APIDOC_EXECUTABLE})
    set(SPHINX_BUILD_CMD ${env_cmd} --xml ${env_xml} ${SPHINX_BUILD_EXECUTABLE})
    
    get_property(proj_python_package_list GLOBAL PROPERTY PROJ_PYTHON_PACKAGE_LIST)
    
    add_custom_target(doc
                      COMMAND  ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/doc/sphinx/html
                      COMMAND  ${SPHINX_BUILD_CMD} . ${_py_pack} html
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/sphinx
                      COMMENT "Generating Sphinx documentation" VERBATIM)
    
    add_dependencies(doc doxygen)



    
    # loop over all python packages found (can have many for each Elements module)
    foreach (_py_pack IN LISTS proj_python_package_list)
    
        get_filename_component(_py_pack_short ${_py_pack} NAME)
        get_filename_component(_py_pack_dir ${_py_pack} PATH)
        get_filename_component(_py_pack_main ${_py_pack_dir} PATH)
        get_filename_component(_el_pack_short ${_py_pack_main} NAME)
        
        set(SPHINX_ELEMENTS_PACK_LIST ${SPHINX_ELEMENTS_PACK_LIST} ${_py_pack_main})        
        
        set(SPHINX_${_el_pack_short}_APIDOC_MODULES "${SPHINX_${_el_pack_short}_APIDOC_MODULES}
   ${_py_pack_short}/modules")

        
        add_custom_target(sphinx_apidoc_${_py_pack_short}
                          COMMAND  ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/doc/sphinx/${_el_pack_short}/${_py_pack_short}
                          COMMAND  ${SPHINX_APIDOC_CMD} -f -o ${PROJECT_BINARY_DIR}/doc/sphinx/${_el_pack_short}/${_py_pack_short} ${_py_pack}
                          COMMENT "Generating Sphinx API documentation for ${_py_pack_short}" VERBATIM)

        add_dependencies(doc sphinx_apidoc_${_py_pack_short})

    endforeach()
    
    if(SPHINX_ELEMENTS_PACK_LIST)
      list(REMOVE_DUPLICATES SPHINX_ELEMENTS_PACK_LIST)
    endif()

 
    #loop over all Elements module
    # this will create an <module>_index.rst for each of them
    foreach(_el_pack IN LISTS SPHINX_ELEMENTS_PACK_LIST)

      get_filename_component(_el_pack_short ${_el_pack} NAME)

      file(GLOB rst-files ${_el_pack}/doc/*.rst)
        
      unset(SPHINX_THIS_RELATED_PAGES)
        
      foreach(r_file ${rst-files})
        get_filename_component(r_file_short ${r_file} NAME)
        get_filename_component(r_file_short_we ${r_file} NAME_WE)      

        if(NOT r_file_short_we STREQUAL "index")      
              
          configure_file(
            "${r_file}"
            "${PROJECT_BINARY_DIR}/doc/sphinx/${_el_pack_short}/${r_file_short}"
            COPYONLY
          )    
          message(STATUS "Copy ${r_file} file to ${PROJECT_BINARY_DIR}/doc/sphinx/${_el_pack_short}/${r_file_short}")

          if(NOT SPHINX_${_el_pack_short}_RELATED_PAGES)
            set(SPHINX_${_el_pack_short}_RELATED_PAGES "
Related Pages
-------------

.. toctree::
   :maxdepth: 2
")  
          endif()
          set(SPHINX_${_el_pack_short}_RELATED_PAGES "${SPHINX_${_el_pack_short}_RELATED_PAGES}
   ${r_file_short_we}")  

        endif()
      endforeach()

      set(SPHINX_THIS_RELATED_PAGES ${SPHINX_${_el_pack_short}_RELATED_PAGES})

      find_file(sphinx_${_el_pack_short}_module_index_file
                NAMES index.rst
                PATHS ${_el_pack}
                PATH_SUFFIXES doc
                NO_DEFAULT_PATH)

      unset(SPHINX_THIS_MODULE_INDEX)
      if(sphinx_${_el_pack_short}_module_index_file)
        file(READ ${sphinx_${_el_pack_short}_module_index_file} SPHINX_THIS_MODULE_INDEX)
        message(STATUS "Using ${sphinx_${_el_pack_short}_module_index_file} for the module ${_el_pack_short}")
      endif()       

      find_file(sphinx_index_module_template
                NAMES index_module.rst.in
                PATHS ${CMAKE_MODULE_PATH}
                PATH_SUFFIXES doc
                NO_DEFAULT_PATH)

      set(SPHINX_THIS_APIDOC_MODULES ${SPHINX_${_el_pack_short}_APIDOC_MODULES})


      if(sphinx_index_module_template)
        configure_file(
                       "${sphinx_index_module_template}"
                       "${PROJECT_BINARY_DIR}/doc/sphinx/${_el_pack_short}/index.rst"
                       @ONLY
                       )
        message(STATUS "Generated Sphinx module index file: ${PROJECT_BINARY_DIR}/doc/sphinx/${_el_pack_short}/index.rst")
        message(STATUS "From the template file: ${sphinx_index_module_template}")
      endif()

      set(SPHINX_EL_MODULES "${SPHINX_EL_MODULES}
   ${_el_pack_short}/index")
    
    endforeach()
    
    
    

    # Generation of the top index.rst file for the project
    find_file(sphinx_main_project_index_file
              NAMES index.rst
              PATHS ${CMAKE_SOURCE_DIR}
              PATH_SUFFIXES doc
              NO_DEFAULT_PATH)

    if(sphinx_main_project_index_file)
      file(READ ${sphinx_main_project_index_file} SPHINX_MAIN_PROJECT_INDEX)
      message(STATUS "Using ${sphinx_main_project_index_file} for the main index of the project")
    endif()


    find_file(sphinx_index_template
              NAMES index.rst.in
              PATHS ${CMAKE_MODULE_PATH}
              PATH_SUFFIXES doc
              NO_DEFAULT_PATH)



    if(sphinx_index_template)
      configure_file(
        "${sphinx_index_template}"
        "${PROJECT_BINARY_DIR}/doc/sphinx/index.rst"
        @ONLY
      )
      message(STATUS "Generated Sphinx main index file: ${PROJECT_BINARY_DIR}/doc/sphinx/index.rst")
      message(STATUS "From the template file: ${sphinx_index_template}")
    endif()
    
  endif()
  