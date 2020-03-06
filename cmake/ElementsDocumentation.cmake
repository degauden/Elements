include_guard()

  if(INSTALL_DOC)
    add_custom_target(doc ALL
                      COMMENT "Generating API documentation" VERBATIM)
  else()
    add_custom_target(doc
                      COMMENT "Generating API documentation" VERBATIM)  
  endif()

#===========================================================================================================

  if(USE_DOXYGEN)


  # Add Doxygen generation
  find_package(Doxygen QUIET)

  if(DOXYGEN_FOUND)

    message(STATUS "Doxygen executable: ${DOXYGEN_EXECUTABLE}")
    message(STATUS "Doxygen version: ${DOXYGEN_VERSION}")

    find_package(PlantUML QUIET)
    
    set(DOXYGEN_EXTRA_FILE_PATTERNS) 
    if(USE_PYTHON_DOXYGEN)
        set(DOXYGEN_EXTRA_FILE_PATTERNS "*.py") 
    endif()


    if(USE_SPHINX)
      set(DOX_LINK_TO_SPHINX "<tab type=\"user\" url=\"../../sphinx/html/index.html\" title=\"Sphinx\"/>")
    else()
      set(DOX_LINK_TO_SPHINX "")
    endif()

    # add the doxygen target
    add_custom_target(doxygen
                      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen/Doxyfile
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen
                      COMMENT "Generating API documentation with Doxygen" VERBATIM)

    add_dependencies(doc doxygen)

    option(DOXYGEN_WITH_CPPREFERENCE_LINKS
           "Link C++ standard library classes to http://cppreference.com documentation."
           TRUE)

    if(DOXYGEN_WITH_CPPREFERENCE_LINKS)
    
      find_file(GET_CPPREF_TAGS_SCRIPT
              get_cppreference_tags.cmake
              PATHS ${CMAKE_MODULE_PATH}
              PATH_SUFFIXES doc)
    
    
      # download Doxygen tags from cppreference.com
      add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen/cppreference-doxygen-web.tag.xml
                         COMMAND ${CMAKE_COMMAND}
                           -D DEST_DIR=${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen
                           -P ${GET_CPPREF_TAGS_SCRIPT}
                         COMMENT "Getting cppreference.com doxygen tags...")
      add_custom_target(get-ccpreference-tags DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen/cppreference-doxygen-web.tag.xml)
      add_dependencies(doxygen get-ccpreference-tags)
      set(DOXYGEN_TAGFILES
        "${DOXYGEN_TAGFILES} \"${CMAKE_CURRENT_BINARY_DIR}/doc/doxygen/cppreference-doxygen-web.tag.xml=http://en.cppreference.com/w/\"")
    endif()

    # Generation of the Doxygen Main Page
    
    list(APPEND maindox_list ${CMAKE_CURRENT_SOURCE_DIR}/doc/mainpage.dox)
    list(APPEND maindox_list ${CMAKE_CURRENT_SOURCE_DIR}/mainpage.dox)
    list(APPEND maindox_list ${CMAKE_CURRENT_SOURCE_DIR}/doc/${PROJECT_NAME}.dox)
    list(APPEND maindox_list ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}.dox)

    set(maindox_file)    
    find_first_file("${maindox_list}" maindox_file)
    
    if(NOT maindox_file)
      message(STATUS "No doxygen main page (${maindox_list}) can be found.")
      find_file_to_configure(mainpage.dox.in
                             FILETYPE "Doxygen main page"
                             OUTPUTDIR "${PROJECT_BINARY_DIR}/doc/doxygen"
                             PATHS ${CMAKE_MODULE_PATH}
                             PATH_SUFFIXES doc)
      set(DOX_INPUT_BINARY_MAIN ${PROJECT_BINARY_DIR}/doc/doxygen/mainpage.dox)
      set(DOX_EXCLUDE_BINARY_DIR)
    else()
      message(STATUS "The ${maindox_file} file exists.")
      set(DOX_EXCLUDE_BINARY_DIR ${PROJECT_BINARY_DIR})
      set(DOX_INPUT_BINARY_MAIN)
    endif()


    # Generation of the main Doxygen configuration: the Doxyfile
    find_file_to_configure(Doxyfile.in
                           FILETYPE "Doxygen configuration"
                           OUTPUTDIR "${PROJECT_BINARY_DIR}/doc/doxygen"
                           PATHS ${CMAKE_MODULE_PATH}
                           PATH_SUFFIXES doc)


    # Generation of the Doxygen Layout
    find_file_to_configure(DoxygenLayout.xml.in
                           FILETYPE "Doxygen layout"
                           OUTPUTDIR "${PROJECT_BINARY_DIR}/doc/doxygen"
                           PATHS ${CMAKE_MODULE_PATH}
                           PATH_SUFFIXES doc)



  endif()

  endif()


#===========================================================================================================


  if(USE_SPHINX AND (NOT "${PYTHON_EXPLICIT_VERSION}" STREQUAL "2"))

  find_package(Sphinx REQUIRED)
  if(SPHINX_FOUND)

    if(NOT SPHINX_BUILD_OPTIONS)
      set(SPHINX_BUILD_OPTIONS "" CACHE STRING "Extra options to pass to sphinx-build" FORCE)
    endif()

    if(NOT SPHINX_APIDOC_OPTIONS)
      set(SPHINX_APIDOC_OPTIONS "" CACHE STRING "Extra options to pass to sphinx-apidoc" FORCE)
    endif()
    
    if(USE_DOXYGEN AND DOXYGEN_FOUND AND USE_SPHINX_BREATHE)
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
                      COMMAND  ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/doc ${PROJECT_BINARY_DIR}/doc/sphinx 
                      COMMAND  ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/doc/sphinx/html
                      COMMAND  ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/doc/sphinx/_static
                      COMMAND  ${SPHINX_BUILD_CMD} ${SPHINX_BUILD_OPTIONS} . ${_py_pack} html
                      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/doc
                      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc/sphinx
                      COMMENT "Generating Sphinx documentation" VERBATIM)

    if(USE_DOXYGEN AND DOXYGEN_FOUND AND USE_SPHINX_APIDOC)
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
    
    # This is the list of Elements modules that do contains python packages
    if(SPHINX_ELEMENTS_PACK_LIST)
      list(REMOVE_DUPLICATES SPHINX_ELEMENTS_PACK_LIST)
    endif()

    get_property(proj_package_list GLOBAL PROPERTY PROJ_PACKAGE_LIST)

    set(_api_modules_line)
    if(USE_SPHINX_APIDOC)
      set(_api_modules_line modules)
    endif()

    #loop over all Elements module
    # this will create an <module>_index.rst for each of them
    foreach(_el_pack IN LISTS proj_package_list)

      get_filename_component(_el_pack_short ${_el_pack} NAME)

      copy_dir(${_el_pack}/doc ${PROJECT_BINARY_DIR}/doc/sphinx/${_el_pack_short})


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
    
    endforeach()


    foreach (_py_pack IN LISTS proj_python_package_list)

      get_filename_component(_py_pack_short ${_py_pack} NAME)
      get_filename_component(_py_pack_dir ${_py_pack} PATH)
      get_filename_component(_py_pack_main ${_py_pack_dir} PATH)
      get_filename_component(_el_pack_short ${_py_pack_main} NAME)

      if(USE_SPHINX_APIDOC)
        if(NOT TARGET sphinx_apidoc_${_py_pack_short})
          add_custom_target(sphinx_apidoc_${_py_pack_short}
                            COMMAND  ${CMAKE_COMMAND} -E make_directory ${PROJECT_BINARY_DIR}/doc/sphinx/${_el_pack_short}
                            COMMAND  ${SPHINX_APIDOC_CMD} ${SPHINX_APIDOC_OPTIONS} -o ${PROJECT_BINARY_DIR}/doc/sphinx/${_el_pack_short} ${_py_pack_dir}
                            COMMENT "Generating Sphinx API documentation for ${_py_pack_short}" VERBATIM)

          add_dependencies(sphinx sphinx_apidoc_${_py_pack_short})

        endif()
      endif()
      


    endforeach()
  



    find_file_to_configure(elements_modules.rst.in
                           FILETYPE "List of Elements modules"
                           OUTPUTDIR "${PROJECT_BINARY_DIR}/doc/sphinx"
                           OUTPUTNAME "elements_modules.rst"
                           PATHS ${CMAKE_MODULE_PATH}
                           PATH_SUFFIXES doc)



    # Generation of the top index.rst file for the project


     if(USE_DOXYGEN AND DOXYGEN_FOUND)
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


  if(INSTALL_DOC)

    install(DIRECTORY ${CMAKE_BINARY_DIR}/doc/
            DESTINATION ${DOC_INSTALL_SUFFIX}
            PATTERN "CVS" EXCLUDE
            PATTERN ".svn" EXCLUDE
            PATTERN "*~" EXCLUDE)

    foreach(_do ChangeLog README README.md)
      find_file(_do_file
                NAMES ${_do}
                PATHS ${CMAKE_SOURCE_DIR}
                PATH_SUFFIXES doc
                NO_DEFAULT_PATH)

      if(_do_file)
          install(FILES ${_do_file}
                  DESTINATION ${DOC_INSTALL_SUFFIX})
      endif()
    endforeach()

  endif()
  
  