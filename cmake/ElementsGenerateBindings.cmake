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

  cmake_parse_arguments(PYBINDGEN "" "MODULE_NAME" "" ${ARGN})

  if(PYBINDGEN_MODULE_NAME)
    set(module_name ${PYBINDGEN_MODULE_NAME})
  else()
    set(module_name ${package}) 
    message(STATUS "Python Bindings Generation: no module name provided. Using the default ${package}")
  endif()

  get_xsd_files(${xsd_dir} xsd_files)

  # get the pyxbgen executable
  find_package(PyXB)
  if(NOT PYXB_FOUND)
    message(FATAL_ERROR "Cannot find the PyXB generator executable")
  endif()
  
  set(xsd_files_full ${xsd_files})
  list(TRANSFORM xsd_files_full PREPEND ${xsd_dir}/)

  set(py_bindings_dir ${CMAKE_BINARY_DIR}/python)

  add_custom_target(PyBindDir ALL ${generate_pybindings_cmd} -o ${py_bindings_dir} -d ${xsd_dir} -m ${module_name} -n ${ns_prefix}
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

  cmake_parse_arguments(CPPBINDGEN "" "INCLUDE_PREFIX;LIBRARY_NAME" "" ${ARGN})

  if(CPPBINDGEN_INCLUDE_PREFIX)
    set(include_prefix ${CPPBINDGEN_INCLUDE_PREFIX})
  else()
    set(include_prefix ${package}) 
    message(STATUS "C++ Bindings Generation: no include prefix provided. Using the default ${package}")
  endif()

  if(CPPBINDGEN_LIBRARY_NAME)
    set(library_name ${CPPBINDGEN_LIBRARY_NAME})
  else()
    set(library_name ${package}) 
    message(STATUS "C++ Bindings Generation: no library name provided. Using the default ${package}")
  endif()

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
    set(xsd_generated_h_file "${cpp_bindings_dir}/include/${include_prefix}/${xsd_base}.h")
    set_source_files_properties(${xsd_generated_cpp_file} ${xsd_generated_h_file} ${xsd_generated_d_file} 
                                PROPERTIES GENERATED TRUE)
    
    if (USED_CMAKE_GENERATOR STREQUAL "Ninja")
      add_custom_command(OUTPUT ${xsd_generated_cpp_file} ${xsd_generated_h_file}
                         MAIN_DEPENDENCY ${xsd_file_full}
                         COMMAND ${generate_cppbinding_cmd} --xsd-executable ${XSDCXX_EXECUTABLE} --dm-dir ${xsd_dir} --h-file ${xsd_generated_h_file} --cpp-file ${xsd_generated_cpp_file} --d-file ${xsd_generated_d_file} --incl-pref ${include_prefix} --namespace-prefix=${ns_prefix} ${xsd_file_full}
                         DEPFILE ${xsd_generated_d_file})
    else()
      add_custom_command(OUTPUT ${xsd_generated_cpp_file} ${xsd_generated_h_file}
                         MAIN_DEPENDENCY ${xsd_file_full}
                         COMMAND ${generate_cppbinding_cmd} --xsd-executable ${XSDCXX_EXECUTABLE} --dm-dir ${xsd_dir} --h-file ${xsd_generated_h_file} --cpp-file ${xsd_generated_cpp_file} --d-file ${xsd_generated_d_file} --incl-pref ${include_prefix} --namespace-prefix=${ns_prefix} ${xsd_file_full}
                         DEPENDS ${xsd_full_files})
    endif()
    
  endforeach()


  find_package(XercesC)

  elements_add_library(${library_name}
                       ${xsd_generated_cpp_files}
                       LINK_LIBRARIES Xsd XercesC
                       INCLUDE_DIRS Xsd ${cpp_bindings_dir}/include
                       NO_PUBLIC_HEADERS)

  include_directories(${cpp_bindings_dir}/include)
  elements_install_headers(${cpp_bindings_dir}/include/${include_prefix} NO_EXIST_CHECK) 
  set_property(GLOBAL APPEND PROPERTY PROJ_HAS_INCLUDE TRUE)
  set_property(GLOBAL APPEND PROPERTY REGULAR_INCLUDE_OBJECTS ${include_prefix})


endfunction()

function(generate_bindings xsd_dir ns_prefix)

  cmake_parse_arguments(BINDGEN "" "CPP_INCLUDE_PREFIX;CPP_LIBRARY_NAME;PYTHON_MODULE_NAME" "" ${ARGN})

  if(BINDGEN_CPP_INCLUDE_PREFIX)
    set(cpp_include_prefix ${BINDGEN_CPP_INCLUDE_PREFIX})
  else()
    set(cpp_include_prefix ${package}) 
    message(STATUS "Bindings Generation: no C++ include prefix provided. Using the default ${package}")
  endif()

  if(BINDGEN_CPP_LIBRARY_NAME)
    set(cpp_library_name ${BINDGEN_CPP_LIBRARY_NAME})
  else()
    set(cpp_library_name ${package}) 
    message(STATUS "Bindings Generation: no C++ library name provided. Using the default ${package}")
  endif()

  generate_cpp_bindings(${xsd_dir} ${ns_prefix}
                        INCLUDE_PREFIX ${cpp_include_prefix}
                        LIBRARY_NAME ${cpp_library_name})
  
  if(BINDGEN_PYTHON_MODULE_NAME)
    set(python_module_name ${BINDGEN_PYTHON_MODULE_NAME})
  else()
    set(python_module_name ${package}) 
    message(STATUS "Bindings Generation: no python module name provided. Using the default ${package}")
  endif()

  generate_python_bindings(${xsd_dir} ${ns_prefix}
                           MODULE_NAME ${python_module_name})

endfunction()

