include_guard()


  add_custom_target(doc
                    COMMENT "Generating API documentation" VERBATIM)


#===========================================================================================================

  if(USE_DOXYGEN)


  # Add Doxygen generation
  find_package(Doxygen QUIET)

  if(DOXYGEN_FOUND)

    # Generation of the main Doxygen configuration: the Doxyfile
    find_file_to_configure(Doxyfile.in
                           FILETYPE "Doxygen configuration"
                           OUTPUTDIR "${PROJECT_BINARY_DIR}/doc/doxygen"
                           PATHS ${CMAKE_MODULE_PATH}
                           PATH_SUFFIXES doc)


    if(USE_SPHINX)
      set(DOX_LINK_TO_SPHINX "<tab type=\"user\" url=\"../../sphinx/html/index.html\" title=\"Sphinx\"/>")
    else()
      set(DOX_LINK_TO_SPHINX "")
    endif()

    # Generation of the Doxygen Layout
    find_file_to_configure(DoxygenLayout.xml.in
                           FILETYPE "Doxygen layout"
                           OUTPUTDIR "${PROJECT_BINARY_DIR}/doc/doxygen"
                           PATHS ${CMAKE_MODULE_PATH}
                           PATH_SUFFIXES doc)


    # add the doxygen target
    add_custom_target(doxygen
                      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen/Doxyfile
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen
                      COMMENT "Generating API documentation with Doxygen" VERBATIM)

    add_dependencies(doc doxygen)


    endif()

  endif()


#===========================================================================================================


  if(USE_SPHINX)

  find_package(Sphinx QUIET)
  if(SPHINX_FOUND)
  
    if(NOT SPHINX_BUILD_OPTIONS)
      set(SPHINX_BUILD_OPTIONS "" CACHE STRING "Extra options to pass to sphinx-build" FORCE)
    endif()
    
    if(NOT SPHINX_APIDOC_OPTIONS)
      set(SPHINX_APIDOC_OPTIONS "" CACHE STRING "Extra options to pass to sphinx-apidoc" FORCE)
    endif()
    
    if(USE_DOXYGEN AND USE_SPHINX_BREATHE)
      set(APPEND_BREATHE_EXT "extensions.append('breathe')")
    else()
      set(APPEND_BREATHE_EXT "")
    endif()

    # Generation of the main sphinx configuration file.
    find_file_to_configure(Sphinx_conf.py.in
                           FILETYPE "Sphinx configuration"
                           OUTPUTDIR "${PROJECT_BINARY_DIR}/doc/sphinx"
                           OUTPUTNAME "conf.py"
                           PATHS ${CMAKE_MODULE_PATH}
                           PATH_SUFFIXES doc)

    configure_files(${CMAKE_CURRENT_SOURCE_DIR}/doc ${PROJECT_BINARY_DIR}/doc/sphinx)


    if(DOXYGEN_FOUND AND USE_SPHINX_APIDOC AND USE_SPHINX_BREATHE)

      find_file_to_configure(cpp_modules.rst.in
                             FILE_TYPE "C++ main ReST"
                             OUTPUTDIR "${PROJECT_BINARY_DIR}/doc/sphinx"
                             PATHS ${CMAKE_MODULE_PATH}
                             PATH_SUFFIXES doc)

       set(SPHINX_CPP_MODULES cpp_modules)
    else()
       set(SPHINX_CPP_MODULES "")
    endif()


    set(SPHINX_APIDOC_CMD ${env_cmd} --xml ${env_xml} ${SPHINX_APIDOC_EXECUTABLE})
    set(SPHINX_BUILD_CMD ${env_cmd} --xml ${env_xml} ${SPHINX_BUILD_EXECUTABLE})

    get_property(proj_python_package_list GLOBAL PROPERTY PROJ_PYTHON_PACKAGE_LIST)

    add_custom_target(sphinx
                      COMMAND  ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/doc/sphinx/html
                      COMMAND  ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/doc/sphinx/_static
                      COMMAND  ${SPHINX_BUILD_CMD} ${SPHINX_BUILD_OPTIONS} . ${_py_pack} html
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/sphinx
                      COMMENT "Generating Sphinx documentation" VERBATIM)

    if(USE_DOXYGEN AND USE_SPHINX_APIDOC)
      add_dependencies(sphinx doxygen)
    endif()
    add_dependencies(doc sphinx)



    # loop over all python packages found (can have many for each Elements module)
    foreach (_py_pack IN LISTS proj_python_package_list)

        get_filename_component(_py_pack_short ${_py_pack} NAME)
        get_filename_component(_py_pack_dir ${_py_pack} PATH)
        get_filename_component(_py_pack_main ${_py_pack_dir} PATH)
        get_filename_component(_el_pack_short ${_py_pack_main} NAME)

        set(SPHINX_ELEMENTS_PACK_LIST ${SPHINX_ELEMENTS_PACK_LIST} ${_py_pack_main})

        if(USE_SPHINX_APIDOC)
        set(SPHINX_${_el_pack_short}_APIDOC_MODULES "${SPHINX_${_el_pack_short}_APIDOC_MODULES}
   ${_py_pack_short}/modules")


        endif()

    endforeach()

    if(SPHINX_ELEMENTS_PACK_LIST)
      list(REMOVE_DUPLICATES SPHINX_ELEMENTS_PACK_LIST)
    endif()


    #loop over all Elements module
    # this will create an <module>_index.rst for each of them
    foreach(_el_pack IN LISTS SPHINX_ELEMENTS_PACK_LIST)

      get_filename_component(_el_pack_short ${_el_pack} NAME)

      configure_files(${_el_pack}/doc ${PROJECT_BINARY_DIR}/doc/sphinx/${_el_pack_short})


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


      if(USE_SPHINX_APIDOC)
      set(SPHINX_THIS_APIDOC_MODULES ${SPHINX_${_el_pack_short}_APIDOC_MODULES})

      set(SPHINX_THIS_PYTHON_PACKAGE "
Python Package
--------------

.. toctree::
   :maxdepth: 4

   ${SPHINX_THIS_APIDOC_MODULES}
"
   )

      endif()

      find_file_to_configure(index_module.rst.in
                             FILETYPE "Sphinx index"
                             OUTPUTDIR "${PROJECT_BINARY_DIR}/doc/sphinx/${_el_pack_short}"
                             OUTPUTNAME "index.rst"
                             PATHS ${CMAKE_MODULE_PATH}
                             PATH_SUFFIXES doc)


    if(NOT SPHINX_EL_MODULES)
      set(SPHINX_EL_MODULES "${_el_pack_short}/index")
    else()
      set(SPHINX_EL_MODULES "${SPHINX_EL_MODULES}
   ${_el_pack_short}/index")
    endif()
    
    if(NOT TARGET sphinx_apidoc_${_el_pack_short})
       add_custom_target(sphinx_apidoc_${_el_pack_short}
                         COMMAND  ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/doc/sphinx/${_el_pack_short}
                         COMMAND  ${SPHINX_APIDOC_CMD} ${SPHINX_APIDOC_OPTIONS} -e -o ${PROJECT_BINARY_DIR}/doc/sphinx/${_el_pack_short} ${_el_pack}/python
                         COMMENT "Generating Sphinx API documentation for ${_el_pack_short}" VERBATIM)

       add_dependencies(sphinx sphinx_apidoc_${_el_pack_short})
    endif()
    
    
    endforeach()


    find_file_to_configure(elements_modules.rst.in
                           FILETYPE "List of Elements modules"
                           OUTPUTDIR "${PROJECT_BINARY_DIR}/doc/sphinx"
                           OUTPUTNAME "elements_modules.rst"
                           PATHS ${CMAKE_MODULE_PATH}
                           PATH_SUFFIXES doc)



    # Generation of the top index.rst file for the project


     if(USE_DOXYGEN)
       set(SPHINX_ORIGINAL_DOX "* The original Doxygen documentation can be accessed with `this link <../../doxygen/html/index.html>`_.")
     else()
       set(SPHINX_ORIGINAL_DOX "")
     endif()


    find_file(sphinx_main_project_index_file
              NAMES index.rst
              PATHS ${CMAKE_SOURCE_DIR}
              PATH_SUFFIXES doc
              NO_DEFAULT_PATH)


    if(sphinx_main_project_index_file)
      configure_file(
                     "${sphinx_main_project_index_file}"
                     "${PROJECT_BINARY_DIR}/doc/sphinx/index.rst"
                     COPYONLY
                    )
    else()
      find_file_to_configure(index.rst.in
                             FILETYPE "Sphinx index"
                             OUTPUTDIR "${PROJECT_BINARY_DIR}/doc/sphinx"
                             OUTPUTNAME "index.rst"
                             PATHS ${CMAKE_MODULE_PATH}
                             PATH_SUFFIXES doc)
    endif()

    endif()

  endif()
