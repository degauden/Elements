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


function(get_xsd_files xsd_dir xsd_files)

  message(STATUS "Parsing XSD directory : ${xsd_dir}")
  message(STATUS "FROM : ${CMAKE_CURRENT_SOURCE_DIR}")
  #list the XSD files

  file(GLOB_RECURSE xsd_files_tmp
       RELATIVE ${xsd_dir}
       ${xsd_dir}/*.xsd
       ${xsd_dir}/*.XSD)
       
   set(${xsd_files} ${xsd_files_tmp} PARENT_SCOPE)
   
endfunction()


function(generate_python_bindings xsd_dir ns_prefix)

  get_xsd_files(${xsd_dir} xsd_files)

  # get the pyxbgen executable
  find_package(PyXB)
  if(NOT PYXB_FOUND)
    message(FATAL_ERROR "Cannot find the PyXB generator executable")
  endif()
  
  set(xsd_files_full ${xsd_files})
  list(TRANSFORM xsd_files_full PREPEND ${xsd_dir}/)

  set(py_bindings_dir ${CMAKE_BINARY_DIR}/python)

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


function(generate_cpp_bindings xsd_dir ns_prefix)

  get_xsd_files(${xsd_dir} xsd_files)
 
  # get the cpp files (generated from XSD)
  set(xsd_generated_cpp_files ${xsd_files})
  set(cpp_bindings_dir ${CMAKE_CURRENT_BINARY_DIR}/cpp_bindings)

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
  elements_install_headers(${cpp_bindings_dir}/include/${package} NO_EXIST_CHECK) 
  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_INCLUDE TRUE)
  set_property(GLOBAL APPEND PROPERTY REGULAR_INCLUDE_OBJECTS ${package})


endfunction()

function(generate_bindings xsd_dir ns_prefix)

  generate_cpp_bindings(${xsd_dir} ${ns_prefix})
  generate_python_bindings(${xsd_dir} ${ns_prefix})

endfunction()

