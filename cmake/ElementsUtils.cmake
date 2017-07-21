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

function(JOIN VALUES GLUE OUTPUT)
  string (REGEX REPLACE "([^\\]|^);" "\\1${GLUE}" _TMP_STR "${VALUES}")
  string (REGEX REPLACE "[\\](.)" "\\1" _TMP_STR "${_TMP_STR}") #fixes escaping
  set (${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
endfunction()


function(get_all_sys_includes inc_list)
    
  set(full_list)
  foreach(d ${CMAKE_PREFIX_PATH})
    if(NOT "${d}" STREQUAL "/")
      set(full_list ${full_list} ${d}/include)
    endif()
  endforeach()

  foreach(d ${CMAKE_SYSTEM_PREFIX_PATH})
    if(NOT "${d}" STREQUAL "/")
      set(full_list ${full_list} ${d}/include)
    endif()
  endforeach()

  foreach(d ${CMAKE_SYSTEM_INCLUDE_PATH})
    if(NOT "${d}" STREQUAL "/")
      set(full_list ${full_list} ${d})
    endif()
  endforeach()

  if(full_list)
    list(REMOVE_DUPLICATES full_list)
  endif()

  set(${inc_list} ${full_list} PARENT_SCOPE)
  
endfunction()


function(is_sys_include is_sys dir)

  set(${is_sys} FALSE PARENT_SCOPE)
  get_all_sys_includes(inc_list)

  list(FIND inc_list ${dir} _index)
   
  if(_index GREATER -1)
    set(${is_sys} TRUE PARENT_SCOPE)
  endif()

endfunction()

function(starts_with_sys_include starts_with_sys dir)

    set(${starts_with_sys} FALSE PARENT_SCOPE)
    get_all_sys_includes(inc_list)

    foreach(_inc_dir ${inc_list})
      if("${dir}" MATCHES "^${_inc_dir}/.*" ) 
        set(${starts_with_sys} TRUE PARENT_SCOPE)
        break()   
      endif()
    endforeach()

endfunction()

#----------------------------------------------------------------
# Filename utils
#----------------------------------------------------------------------

function(split_filename_ext filename_ne filename_ext filename)
  string(REGEX MATCH "^(.*)\\.([^.]*)$" dummy ${filename})

  set(${filename_ne} ${CMAKE_MATCH_1} PARENT_SCOPE)
  set(${filename_ext} ${CMAKE_MATCH_2} PARENT_SCOPE)
endfunction()

function(find_file_to_configure template_file_name)

  CMAKE_PARSE_ARGUMENTS(TEMPLATE_CONF "" "OUTPUTDIR;OUTPUTNAME;FILETYPE" "PATHS;PATH_SUFFIXES" ${ARGN})

  # needed to reset the cache variable.
  set(template_file template_file-NOTFOUND)

  find_file(template_file
            NAMES ${template_file_name}
            PATHS ${TEMPLATE_CONF_PATHS}
            PATH_SUFFIXES ${TEMPLATE_CONF_PATH_SUFFIXES}
            NO_DEFAULT_PATH)

  if(template_file)

    split_filename_ext(template_filename_ne template_filename_ext ${template_file_name})

    if(NOT TEMPLATE_CONF_OUTPUTNAME)
      split_filename_ext(template_filename_ne template_filename_ext ${template_file_name})
      set(TEMPLATE_CONF_OUTPUTNAME ${template_filename_ne})
    endif()

    configure_file("${template_file}"
                   "${TEMPLATE_CONF_OUTPUTDIR}/${TEMPLATE_CONF_OUTPUTNAME}"
                   @ONLY)
    message(STATUS "Generated the ${TEMPLATE_CONF_FILETYPE} file: ${TEMPLATE_CONF_OUTPUTDIR}/${TEMPLATE_CONF_OUTPUTNAME}")
    message(STATUS "From the template file: ${template_file}")
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

macro(elements_expand_source_dirs VAR)
  set(${VAR})
  foreach(fp ${ARGN})
    file(GLOB files ${fp}/*)
    if(files)
      set(${VAR} ${${VAR}} ${files})
    endif()
  endforeach()
endmacro()

# extract the list of subdirectories containing files recursively
macro(elements_recurse_dirs VAR)
  set(${VAR})
  foreach(fp ${ARGN})
  
    file(GLOB_RECURSE files ${fp}/*)
    set(dir_list)
    foreach(file_path ${files})
      get_filename_component(dir_path ${file_path} PATH)
      set(dir_list ${dir_list} ${dir_path})
    endforeach()
    list(REMOVE_DUPLICATES dir_list)
    set(${VAR} ${${VAR}} ${dir_list})  
  endforeach()
endmacro()

macro(elements_recurse_cython_include_dirs VAR)
  set(${VAR})
  foreach(fp ${ARGN})
  
    file(GLOB_RECURSE files ${fp}/*.px[di])
    set(dir_list)
    foreach(file_path ${files})
      get_filename_component(dir_path ${file_path} PATH)
      set(dir_list ${dir_list} ${dir_path})
    endforeach()
    if(dir_list)
      list(REMOVE_DUPLICATES dir_list)
    endif()
    set(${VAR} ${${VAR}} ${dir_list})  
  endforeach()
endmacro()

function(elements_recurse result)

  CMAKE_PARSE_ARGUMENTS(ARG "" "PATTERN" "" ${ARGN})
    
    
  if(NOT ARG_PATTERN)
    set(ARG_PATTERN "*")
  endif()
  
  set(total_dir_list)
  
  foreach(fp ${ARG_UNPARSED_ARGUMENTS})
    file(GLOB_RECURSE files ${fp}/${ARG_PATTERN})
    set(dir_list)
    foreach(file_path ${files})
      get_filename_component(dir_path ${file_path} PATH)
      set(dir_list ${dir_list} ${dir_path})
    endforeach()
    if(dir_list)
      list(REMOVE_DUPLICATES dir_list)
    endif()
    set(total_dir_list ${total_dir_list} ${dir_list})  
  endforeach()
  
  if(total_dir_list)
    list(REMOVE_DUPLICATES total_dir_list)
  endif()
  
  set(${result} ${total_dir_list} PARENT_SCOPE)

endfunction()


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

# Copy files from source directory to destination directory, substituting any
# variables.  Create destination directory if it does not exist.

macro(configure_files srcDir destDir)
    message(STATUS "Configuring directory ${destDir}")
    file(MAKE_DIRECTORY ${destDir})
    file(GLOB templateFiles RELATIVE ${srcDir} ${srcDir}/*)
    foreach(templateFile ${templateFiles})
        set(srcTemplatePath ${srcDir}/${templateFile})
        if(NOT IS_DIRECTORY ${srcTemplatePath})
            message(STATUS "Configuring file ${templateFile}")
            configure_file(
                    ${srcTemplatePath}
                    ${destDir}/${templateFile}
                    @ONLY)
        endif(NOT IS_DIRECTORY ${srcTemplatePath})
    endforeach(templateFile)
endmacro(configure_files)

macro(copy_dir srcDir destDir)
    message(STATUS "Copying directory the content of ${srcDir} to ${destDir}")
    file(MAKE_DIRECTORY ${destDir})
    file(GLOB templateFiles RELATIVE ${srcDir} ${srcDir}/*)
    foreach(templateFile ${templateFiles})
        set(srcTemplatePath ${srcDir}/${templateFile})
            message(STATUS "Copying ${templateFile}")
            file(COPY ${srcTemplatePath} DESTINATION ${destDir})
    endforeach(templateFile)
endmacro(copy_dir)

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

  set(the_list)

  list(APPEND the_list ${project}/${version})
  list(APPEND the_list ${project}_${version})

  if(NOT ELEMENTS_USE_CASE_SENSITIVE_PROJECTS)
    string(TOUPPER ${project} project_upcase)
    list(APPEND the_list ${project_upcase}/${project_upcase}_${version})
  endif()

  list(REMOVE_DUPLICATES the_list)

  set(${full_list} ${the_list} PARENT_SCOPE)

endfunction()


function(get_versionless_project_bases project full_list)

  set(the_list)

  list(APPEND the_list ${project})

  if(NOT ELEMENTS_USE_CASE_SENSITIVE_PROJECTS)
    string(TOUPPER ${project} project_upcase)
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



function(get_installed_versionless_project_suffixes project binary_tag binary_base suffixes)

  get_full_binary_list(${binary_tag} ${binary_base} full_binary_list)

  get_versionless_project_bases(${project} full_bases_list)

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


function(get_rpm_dep_list project_use package_suffix squeezed_install output_var)
  
  set(output_str_list)

  set(ARGN_ ${project_use})

  while(ARGN_)
    list(LENGTH ARGN_ len)
    if(len LESS 2)
      message(FATAL_ERROR "Wrong number of arguments to USE option")
    endif()
    list(GET ARGN_ 0 other_project)
    list(GET ARGN_ 1 other_project_version)

    if(squeezed_install)
      if(package_suffix)
	set(output_str_list "${output_str_list}, ${other_project}-${package_suffix} = ${other_project_version}")
      else()
	set(output_str_list "${output_str_list}, ${other_project} = ${other_project_version}")
      endif()
    else()
      if(package_suffix)
	set(output_str_list "${output_str_list}, ${other_project}_${other_project_version}-${package_suffix}")
      else()
	set(output_str_list "${output_str_list}, ${other_project}_${other_project_version}")
      endif()
    endif()
    
    
    list(REMOVE_AT ARGN_ 0 1)
  endwhile()

  if(NOT squeezed_install)
    if(package_suffix STREQUAL "")
      set(output_str_list "${output_str_list}, EuclidEnv")
    endif()
  endif()
    
  set(${output_var} ${output_str_list} PARENT_SCOPE)

endfunction()


function(get_rpm_dep_lines project_use package_suffix squeezed_install line_prefix output_var)
  
  set(output_str_lines)

  set(ARGN_ ${project_use})
    
  while(ARGN_)
    list(LENGTH ARGN_ len)
    if(len LESS 2)
      message(FATAL_ERROR "Wrong number of arguments to USE option")
    endif()
    list(GET ARGN_ 0 other_project)
    list(GET ARGN_ 1 other_project_version)

    set(other_proj_pack_name)
    if(squeezed_install)
      set(other_proj_pack_name "${other_project}")
    else()
      set(other_proj_pack_name "${other_project}_${other_project_version}")
    endif()
    
    if(package_suffix)
	  set(other_proj_pack_name "${other_proj_pack_name}-${package_suffix}")
    endif()
 
    if(squeezed_install)
      set(other_proj_pack_line "${other_proj_pack_name} = ${other_project_version}")
    else()
      set(other_proj_pack_line "${other_proj_pack_name}")
    endif()
 
    if(line_prefix)
      set(other_proj_pack_line "${line_prefix}: ${other_proj_pack_line}")  
    endif()

    
    if( "${output_str_lines}" STREQUAL "")
      set(output_str_lines "${other_proj_pack_line}")
    else()
      set(output_str_lines "${output_str_lines}
${other_proj_pack_line}")    
    endif()

    list(REMOVE_AT ARGN_ 0 1)
  endwhile()
    
  set(${output_var} ${output_str_lines} PARENT_SCOPE)

endfunction()

function(get_rpm_sys_dep_lines dep_list line_prefix output_var)
  
  set(output_str_lines)
  
  list(REMOVE_DUPLICATES dep_list)
  
  foreach(other_sys_pack_line ${dep_list})
  
    debug_print_var(other_sys_pack_line)
    if(line_prefix)
      set(other_sys_pack_line "${line_prefix}: ${other_sys_pack_line}")  
    endif()
    
    if( "${output_str_lines}" STREQUAL "")
      set(output_str_lines "${other_sys_pack_line}")
    else()
      set(output_str_lines "${output_str_lines}
${other_sys_pack_line}")    
    endif()

  endforeach()
  
  set(${output_var} ${output_str_lines} PARENT_SCOPE)

  debug_print_var(output_str_lines)

endfunction()

function(get_arch_lib_dir output_var)

  if(EXISTS /usr/lib64)
    set(lib_name lib64)
  else()
    set(lib_name lib)
  endif()

  set(${output_var} ${lib_name} PARENT_SCOPE)

endfunction()


macro(print_all_variables)
  get_cmake_property(_variableNames VARIABLES)
  foreach (_variableName ${_variableNames})
    message(STATUS "${_variableName}=${${_variableName}}")
  endforeach()
endmacro()



function(find_python_module module)

    find_package(PythonInterp ${PYTHON_EXPLICIT_VERSION})

    string(TOUPPER ${module} module_upper)
    if(NOT PY_${module_upper})
        if(ARGC GREATER 1 AND ARGV1 STREQUAL "REQUIRED")
            set(${module}_FIND_REQUIRED TRUE)
        endif()
        # A module's location is usually a directory, but for binary modules
        # it's a .so file.
        execute_process(COMMAND "${PYTHON_EXECUTABLE}" "-c"
            "import re, ${module}; print(re.compile('/__init__.py.*').sub('',${module}.__file__))"
            RESULT_VARIABLE _${module}_status
            OUTPUT_VARIABLE _${module}_location
            ERROR_QUIET
            OUTPUT_STRIP_TRAILING_WHITESPACE)
        if(NOT _${module}_status)
            set(PY_${module_upper} ${_${module}_location} CACHE STRING
                "Location of Python module ${module}")
        endif(NOT _${module}_status)
    endif(NOT PY_${module_upper})
    find_package_handle_standard_args(PY_${module} DEFAULT_MSG PY_${module_upper})

endfunction(find_python_module)


