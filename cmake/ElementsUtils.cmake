if (NOT HAS_ELEMENTS_UTILS)


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

  set(the_list "")

  if(NOT binary_tag STREQUAL "")
    set(the_list ${the_list} "${binary_tag}")
  endif()

  message(STATUS "Elements use strict binary dependencies: ${ELEMENTS_USE_STRICT_BINARY_DEP}")

  if(SGS_BUILD_TYPE_SHORT_NAMES AND NOT ELEMENTS_USE_STRICT_BINARY_DEP)
    foreach(_s3 ${SGS_BUILD_TYPE_SHORT_NAMES})
      set(the_list ${the_list} "${binary_base}-${_s3}")
    endforeach()
  endif()

  list(REMOVE_DUPLICATES the_list)

  set(${full_list} ${the_list} PARENT_SCOPE)

endfunction()

function(get_project_bases project version full_list)

  set(the_list ${project})
  if(NOT ELEMENTS_USE_CASE_SENSITIVE_PROJECTS)
    string(TOUPPER ${project} project_upcase)
    set(the_list ${the_list} ${project_upcase}/${project_upcase}_${version})
  endif()

  set(the_list ${the_list} ${project}/${version})

  if(NOT ELEMENTS_USE_CASE_SENSITIVE_PROJECTS)
    set(the_list ${the_list} ${project_upcase})
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
          set(the_list ${the_list} ${_s1}/${_s2}/${_s3})
        else()
          set(the_list ${the_list} ${_s1}/${_s3})
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
      set(the_list ${the_list} ${_s1}/${install_base}/${_s3})
    endforeach()
  endforeach()

  list(REMOVE_DUPLICATES the_list)

  set(${suffixes} ${the_list} PARENT_SCOPE)

endfunction()



function(find_local_project project version path_list proj_loc)
  get_local_project_suffixes(${project} ${version} suffixes)
  find_path(this_location CMakeLists.txt
            HINTS ${path_list}
            PATH_SUFFIXES ${suffixes})


  set(${proj_loc} ${this_location} PARENT_SCOPE)

endfunction()



function(find_installed_project project version binary_tag binary_base proj_loc)
  get_local_project_suffixes(${project} ${version} ${binary_tag} ${binary_base} suffixes)



endfunction()


# check if the project is already present in the dependency
# tree. If so, and if the version is not the same, it breaks
function(check_project_consistency project version dep_tree)

endfunction()

# recursing function to find all the dependencies
function(find_project_dependency_tree project version path_list dep_tree)

endfunction()




set(HAS_ELEMENTS_UTILS ON)
endif()
