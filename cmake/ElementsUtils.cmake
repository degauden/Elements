macro(include_guard)
  get_filename_component(file_to_include ${CMAKE_CURRENT_LIST_FILE} NAME_WE)
  string(TOUPPER ${file_to_include} file_to_include_upcase)
  if(${file_to_include_upcase}_IS_INCLUDED)
    return()
  else()
    set(${file_to_include_upcase}_IS_INCLUDED 1)
    set(FULL_INCLUDE_FILE_LIST ${FULL_INCLUDE_FILE_LIST} ${file_to_include_upcase}_IS_INCLUDED)
  endif()
endmacro()

include_guard()

macro(reset_include_guards)

  foreach(_s1 ${FULL_INCLUDE_FILE_LIST})
    set(${_s1} 0)
  endforeach()

endmacro()

# Options
option(USE_DEBUG_PRINT
       "make the debug_print_var macro talkative"
       ON)


macro(debug_print_var var)
  if(USE_DEBUG_PRINT)
    message(STATUS "${var} -> ${${var}}")
  endif()
endmacro()


macro(debug_message)
  if(USE_DEBUG_PRINT)
    message(${ARGN})
  endif()
endmacro()


set(FULL_MESSAGE_LIST "" CACHE INTERNAL "This is the full list of guarded messages")

macro(guarded_message)
  string(MD5 this_guard "${ARGN}")
  if(NOT ${this_guard})
    set(${this_guard} TRUE CACHE INTERNAL "")
    set(FULL_MESSAGE_LIST ${FULL_MESSAGE_LIST} ${this_guard})
    message(${ARGN})
  endif()
endmacro()


macro(reset_message_guards)

  foreach(_s1 ${FULL_MESSAGE_LIST})
    set(${_s1} FALSE CACHE INTERNAL "")
  endforeach()

endmacro()


function(recurse_test nb)
  message(STATUS "This is the number ${nb}")
  math(EXPR nb "${nb}-1")
  if(nb GREATER 0)
    recurse_test(${nb})
  endif()
endfunction()



#-------------------------------------------------------------------------------
# elements_expand_sources(<variable> source_pattern1 source_pattern2 ...)
#
# Expand glob patterns for input files to a list of files, first searching in
# ``src`` then in the current directory.
#-------------------------------------------------------------------------------
macro(elements_expand_sources VAR)
  set(${VAR})
  foreach(fp ${ARGN})
    file(GLOB files src/${fp})
    if(files)
      set(${VAR} ${${VAR}} ${files})
    else()
      file(GLOB files ${fp})
      if(files)
        set(${VAR} ${${VAR}} ${files})
      else()
        set(${VAR} ${${VAR}} ${fp})
      endif()
    endif()
  endforeach()
endmacro()

function(filter_comments var)
  # Convert file contents into a CMake list (where each element in the list
  # is one line of the file)
  #
  STRING(REGEX REPLACE ";" "\\\\;" contents2 "${${var}}")
  STRING(REGEX REPLACE "\n" ";" contents2 "${contents2}")
  foreach(__t ${contents2})
    if (NOT "${__t}" MATCHES "^ *#+")
      LIST(APPEND contents3 ${__t})
    endif()
  endforeach()
  STRING(REGEX REPLACE ";" "\n" contents3 "${contents3}")
  STRING(REGEX REPLACE "\\\\;" ";" contents3 "${contents3}")
  set(${var} ${contents3} PARENT_SCOPE)
endfunction()

#-------------------------------------------------------------------------------
# Functions for the crawling of the project dependence tree
#-------------------------------------------------------------------------------


function(get_full_binary_list binary_tag binary_base full_list)

  if(NOT binary_tag STREQUAL "")
    list(APPEND the_list "${binary_tag}")
  endif()

  guarded_message(STATUS "Elements use strict binary dependencies: ${ELEMENTS_USE_STRICT_BINARY_DEP}")

  if(SGS_BUILD_TYPE_SHORT_NAMES AND NOT ELEMENTS_USE_STRICT_BINARY_DEP)
    foreach(_s3 ${SGS_BUILD_TYPE_SHORT_NAMES})
      list(APPEND the_list "${binary_base}-${_s3}")
    endforeach()
  endif()

  list(REMOVE_DUPLICATES the_list)

  set(${full_list} ${the_list} PARENT_SCOPE)

endfunction()

function(get_project_bases project version full_list)

  set(the_list ${project})
  if(NOT ELEMENTS_USE_CASE_SENSITIVE_PROJECTS)
    string(TOUPPER ${project} project_upcase)
    list(APPEND the_list ${project_upcase}/${project_upcase}_${version})
  endif()

  list(APPEND the_list ${project}/${version})

  if(NOT ELEMENTS_USE_CASE_SENSITIVE_PROJECTS)
    list(APPEND the_list ${project_upcase})
  endif()

  list(REMOVE_DUPLICATES the_list)

  set(${full_list} ${the_list} PARENT_SCOPE)

endfunction()


function(get_project_suffixes project version binary_tag binary_base suffixes)

  get_full_binary_list(${binary_tag} ${binary_base} full_binary_list)

  get_project_bases(${project} ${version} full_bases_list)

  set(install_base "" "InstallArea")

  set(the_list)

  foreach(_s1 ${full_bases_list})
    foreach(_s2 ${install_base})
      foreach(_s3 ${full_binary_list})
        if(_s2)
          list(APPEND the_list ${_s1}/${_s2}/${_s3})
        else()
          list(APPEND the_list ${_s1}/${_s3})
        endif()
      endforeach()
    endforeach()
  endforeach()

  list(REMOVE_DUPLICATES the_list)

  set(${suffixes} ${the_list} PARENT_SCOPE)

endfunction()



function(get_local_project_suffixes project version suffixes)

  get_project_bases(${project} ${version} full_bases_list)

  list(REMOVE_DUPLICATES full_bases_list)

  set(${suffixes} ${full_bases_list} PARENT_SCOPE)

endfunction()



function(get_installed_project_suffixes project version binary_tag binary_base suffixes)

  get_full_binary_list(${binary_tag} ${binary_base} full_binary_list)

  get_project_bases(${project} ${version} full_bases_list)

  set(install_base "InstallArea")

  set(the_list)

  foreach(_s1 ${full_bases_list})
    foreach(_s3 ${full_binary_list})
      list(APPEND the_list ${_s1}/${install_base}/${_s3})
    endforeach()
  endforeach()

  list(REMOVE_DUPLICATES the_list)

  set(${suffixes} ${the_list} PARENT_SCOPE)

endfunction()



function(find_local_project project version path_list proj_loc)
  get_local_project_suffixes(${project} ${version} suffixes)

  debug_message(STATUS "This are the suffixes: ${suffixes}")
  debug_message(STATUS "This are the paths: ${path_list}")

  find_path(this_location CMakeLists.txt
            HINTS ${path_list}
            PATH_SUFFIXES ${suffixes})


  debug_message(STATUS "This are the proj location: ${this_location}")


  check_local_project_version(${project} ${version} ${this_location})

  set(${proj_loc} ${this_location} PARENT_SCOPE)
endfunction()



function(find_installed_project project version binary_tag binary_base path_list proj_loc)
  get_local_project_suffixes(${project} ${version} ${binary_tag} ${binary_base} suffixes)

  find_path(this_location manifest.xml
            HINTS ${path_list}
            PATH_SUFFIXES ${suffixes})


  check_installed_project_version(${project} ${version} ${this_location})

  set(${proj_loc} ${this_location} PARENT_SCOPE)
endfunction()


# check if the project is already present in the dependency
# tree. If so, and if the version is not the same, it breaks
function(check_project_consistency project version dep_tree mismatch)

  list(LENGTH dep_tree len)
  if(len LESS 2)
    message(FATAL_ERROR "Wrong number of item in the dependency tree")
  endif()

  set(dependee_list ${dep_tree})
  math(EXPR half_len "${len}/2")
  foreach(index RANGE 1 ${half_len})
    list(GET dependee_list 0 other_proj)
    list(GET dependee_list 1 other_proj_version)
    list(REMOVE_AT dependee_list 0 1)
    if( "${project}" STREQUAL "${other_proj}")
      string(COMPARE NOTEQUAL "${version}" "${other_proj_version}" ver_mismatch)
    endif()
  endforeach()

  set(${mismatch} ${ver_mismatch} PARENT_SCOPE)

endfunction()

# recursing function to find all the dependencies
function(find_project_dependency_tree project version path_list dep_tree)



endfunction()








function(get_project_from_file config_file project version dep_list)

  get_filename_component(cfg_file ${config_file} NAME)

  file(READ ${config_file} config_file_data)
  filter_comments(config_file_data)

  if(cfg_file STREQUAL "CMakeLists.txt")

    string(REGEX MATCH "[ \t]*(elements_project)[ \t]*\\(([^)]+)\\)" match_use ${config_file_data})
    set(match_use ${CMAKE_MATCH_2})

    if(match_use STREQUAL "")
      message(FATAL_ERROR "${config_file} does not contain elements_project")
    endif()

    # (replace space-type chars with spaces)
    string(REGEX REPLACE "[ \t\r\n]+" " " args "${match_use}")
    separate_arguments(args)
    CMAKE_PARSE_ARGUMENTS(PROJECT "" "" "USE;DATA;DESCRIPTION" ${args})

    # get the project name and add it to the list of used projects
    list(GET PROJECT_UNPARSED_ARGUMENTS 0 proj_name)
    set(${project} ${proj_name} PARENT_SCOPE)

    list(GET PROJECT_UNPARSED_ARGUMENTS 1 vers_name)
    set(${version} ${vers_name} PARENT_SCOPE)

    set(${dep_list} ${PROJECT_USE} PARENT_SCOPE)

  elseif(cfg_file MATCHES "([^)]+)Config.cmake")

    string(REGEX MATCH "([^)]+)Config.cmake" match_proj ${cfg_file})
    set(match_proj ${CMAKE_MATCH_1})
    set(${project} ${match_proj} PARENT_SCOPE)

    string(REGEX MATCH "[ \t]*(set)[ \t]*\\(([^)]+_VERSION[^)]+)\\)" match_use ${config_file_data})
    set(match_use ${CMAKE_MATCH_2})
    if(match_use STREQUAL "")
      message(FATAL_ERROR "${config_file} does not contain any version")
    endif()
    # (replace space-type chars with spaces)
    string(REGEX REPLACE "[ \t\r\n]+" " " args "${match_use}")
    separate_arguments(args)
    CMAKE_PARSE_ARGUMENTS(PROJECT "" "" "${match_proj}_VERSION" ${args})
    set(${version} ${PROJECT_${match_proj}_VERSION} PARENT_SCOPE)

    string(REGEX MATCH "[ \t]*(set)[ \t]*\\(([^)]+_USES[^)]+)\\)" match_use ${config_file_data})
    set(match_use ${CMAKE_MATCH_2})
    # (replace space-type chars with spaces)
    string(REGEX REPLACE "[ \t\r\n]+" " " args "${match_use}")

    if(match_use STREQUAL "")
      message(FATAL_ERROR "${config_file} does not contain any uses")
    endif()
    separate_arguments(args)
    CMAKE_PARSE_ARGUMENTS(PROJECT "" "" "${match_proj}_USES" ${args})
    set(${dep_list} ${PROJECT_${match_proj}_USES} PARENT_SCOPE)



  endif()

endfunction()


function(check_project_version_from_file config_file project version match_found)

  set(has_found FALSE)

  get_project_from_file(${config_file} file_project_name file_version_name file_project_dep_list)

  if( (project STREQUAL file_project_name) AND (version STREQUAL file_version_name) )
    set(has_found TRUE)
  endif()

  set(${match_found} ${has_found} PARENT_SCOPE)

endfunction()
