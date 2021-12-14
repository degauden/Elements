include_guard(GLOBAL)

include(CMakeParseArguments)
include(ElementsUtils)

find_program(generate_cppbinding_cmd generateCppBinding.py HINTS ${binary_paths})
if(generate_cppbinding_cmd)
  set(generate_cppbinding_cmd ${PYTHON_EXECUTABLE} ${generate_cppbinding_cmd})
endif()

find_program(generate_pybindings_cmd generate_pybindings.py HINTS ${binary_paths})
if(generate_pybindings_cmd)
  set(generate_pybindings_cmd ${PYTHON_EXECUTABLE} ${generate_pybindings_cmd})
endif()

set(cpp_bindings_dir ${CMAKE_CURRENT_BINARY_DIR}/cpp_bindings)
set(generate_flag ${CMAKE_CURRENT_BINARY_DIR}/generate_flag)
set(bindings_source_ready ${CMAKE_CURRENT_BINARY_DIR}/bindings_source_ready)

set(py_bindings_dir ${CMAKE_BINARY_DIR}/python)

function(local_install_headers)

  CMAKE_PARSE_ARGUMENTS(ARG "NO_EXIST_CHECK" "" "" ${ARGN})

  set(has_local_headers FALSE)
  foreach(hdr_dir ${ARG_UNPARSED_ARGUMENTS})
    if(NOT IS_ABSOLUTE ${hdr_dir})
      set(full_hdr_dir ${CMAKE_CURRENT_SOURCE_DIR}/${hdr_dir})
    else()
      set(full_hdr_dir ${hdr_dir})
    endif()
    if(ARG_NO_EXIST_CHECK)
      install(DIRECTORY ${hdr_dir}
              DESTINATION ${INCLUDE_INSTALL_SUFFIX}
              FILES_MATCHING
              PATTERN "*.h"
              PATTERN "*.icpp"
              PATTERN "*.hpp"
              PATTERN "*.hxx"
              PATTERN "*.i"
              PATTERN "*.pxd"
              PATTERN "CVS" EXCLUDE
              PATTERN ".svn" EXCLUDE)
      if(NOT IS_ABSOLUTE ${hdr_dir})
        set(has_local_headers TRUE)
        set_property(GLOBAL APPEND PROPERTY REGULAR_INCLUDE_OBJECTS ${hdr_dir})
      endif()    
    else()
      if(IS_DIRECTORY ${full_hdr_dir})
        install(DIRECTORY ${hdr_dir}
                DESTINATION ${INCLUDE_INSTALL_SUFFIX}
                FILES_MATCHING
                PATTERN "*.h"
                PATTERN "*.icpp"
                PATTERN "*.hpp"
                PATTERN "*.hxx"
                PATTERN "*.i"
                PATTERN "*.pxd"
                PATTERN "CVS" EXCLUDE
                PATTERN ".svn" EXCLUDE)
        if(NOT IS_ABSOLUTE ${hdr_dir})
          set(has_local_headers TRUE)
          set_property(GLOBAL APPEND PROPERTY REGULAR_INCLUDE_OBJECTS ${hdr_dir})
        endif()
      else()
        message(FATAL_ERROR "No ${hdr_dir} directory for header files in the ${CMAKE_CURRENT_SOURCE_DIR} location")
      endif()
    endif()
  endforeach()
  # flag the current directory as one that installs headers
  #   the property is used when collecting the include directories for the
  #   dependent subdirs
  if(has_local_headers)
    set_property(DIRECTORY PROPERTY INSTALLS_LOCAL_HEADERS TRUE)
    set_property(GLOBAL APPEND PROPERTY PROJ_HAS_INCLUDE TRUE)
  endif()
endfunction()

function(generate_bindings xsd_dir ns_prefix)

  message(STATUS "Parsing XSD directory : ${xsd_dir}")
  message(STATUS "FROM : ${CMAKE_CURRENT_SOURCE_DIR}")
  #list the XSD files

  file(GLOB_RECURSE xsd_files
       RELATIVE ${xsd_dir}
       ${xsd_dir}/*.xsd
       ${xsd_dir}/*.XSD)
 

  # get the cpp files (generated from XSD)
  set(xsd_generated_cpp_files ${xsd_files})
  list(TRANSFORM xsd_generated_cpp_files PREPEND ${cpp_bindings_dir}/src/${package}/)
  list(TRANSFORM xsd_generated_cpp_files REPLACE "\\.xsd$" ".cpp")
  list(TRANSFORM xsd_generated_cpp_files REPLACE "\\.XSD$" ".cpp")
  set_source_files_properties(${xsd_generated_cpp_files} PROPERTIES GENERATED TRUE)


  # get the xsdcxx executable
  find_package(Xsd)
  if(NOT XSD_FOUND)
    message(FATAL_ERROR "Cannot find the Xsd generator executable")
  endif()

  message(STATUS "looping over ${xsd_files}")
  foreach(xsd_file ${xsd_files})
    message(STATUS "treating XSD file : ${xsd_file}")
    set(xsd_file_full "${xsd_dir}/${xsd_file}")
    string(REPLACE ".xsd" "" xsd_base "${xsd_file}")
    
    set(xsd_generated_cpp_file "${cpp_bindings_dir}/src/${package}/${xsd_base}.cpp")
    set(xsd_generated_d_file "${cpp_bindings_dir}/src/${package}/${xsd_base}.d")
    set(xsd_generated_h_file "${cpp_bindings_dir}/include/${package}/${xsd_base}.h")
    set_source_files_properties(${xsd_generated_cpp_file} ${xsd_generated_h_file} ${xsd_generated_d_file} 
                                PROPERTIES GENERATED TRUE)
    
    if (USED_CMAKE_GENERATOR STREQUAL "Ninja")
      add_custom_command(OUTPUT ${xsd_generated_cpp_file} ${xsd_generated_h_file}
                         MAIN_DEPENDENCY ${xsd_file_full}
                         COMMAND ${generate_cppbinding_cmd} --xsd-executable ${XSDCXX_EXECUTABLE} --dm-dir ${xsd_dir} --h-file ${xsd_generated_h_file} --cpp-file ${xsd_generated_cpp_file} --d-file ${xsd_generated_d_file} --incl-pref ${package} --namespace-prefix=${ns_prefix} ${xsd_file_full}
                         DEPFILE ${xsd_generated_d_file})
    else()
      add_custom_command(OUTPUT ${xsd_generated_cpp_file} ${xsd_generated_h_file}
                         MAIN_DEPENDENCY ${xsd_file_full}
                         COMMAND ${generate_cppbinding_cmd} --xsd-executable ${XSDCXX_EXECUTABLE} --dm-dir ${xsd_dir} --h-file ${xsd_generated_h_file} --cpp-file ${xsd_generated_cpp_file} --d-file ${xsd_generated_d_file} --incl-pref ${package} --namespace-prefix=${ns_prefix} ${xsd_file_full}
                         DEPENDS ${xsd_full_files})
    endif()
    
  endforeach()


  find_package(XercesC)

  elements_add_library(${package}
                       ${xsd_generated_cpp_files}
                       LINK_LIBRARIES Xsd XercesC
                       INCLUDE_DIRS Xsd ${cpp_bindings_dir}/include
                       NO_PUBLIC_HEADERS)

  include_directories(${cpp_bindings_dir}/include)
  local_install_headers(${cpp_bindings_dir}/include/${package} NO_EXIST_CHECK) 
  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_INCLUDE TRUE)
  set_property(GLOBAL APPEND PROPERTY REGULAR_INCLUDE_OBJECTS ${package})


  # get the pyxbgen executable
  find_package(PyXB)
  if(NOT PYXB_FOUND)
    message(FATAL_ERROR "Cannot find the PyXB generator executable")
  endif()
  
  set(xsd_files_full ${xsd_files})
  list(TRANSFORM xsd_files_full PREPEND ${xsd_dir}/)

  add_custom_target(PyBindDir ALL ${generate_pybindings_cmd} -o ${py_bindings_dir} -d ${xsd_dir} -m ${package} -n ${ns_prefix}
                    DEPENDS ${xsd_files_full})

  install(DIRECTORY ${py_bindings_dir}/
          DESTINATION "python"
          FILES_MATCHING
          PATTERN "*"
          PATTERN "*.pyc" EXCLUDE
          PATTERN "CVS" EXCLUDE
          PATTERN ".svn" EXCLUDE)


endfunction()

