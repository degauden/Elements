recurse_test(19)


set(project toto)
set(version 1.0)

debug_message(STATUS "======================================================================")

get_full_binary_list(x86_64-fc21-gcc49-opt x86_64-fc21-gcc49 full_list)

debug_message(STATUS "------>  this is the full binary list ${full_list}")

list(LENGTH full_list len)

debug_message(STATUS "------>  this is the length of the full binary list ${len}")


debug_message(STATUS "======================================================================")


set(dep_tree "bla;3.2;toto;1.0;titi;2.1;tutu;4.9")

debug_message(STATUS "------>  this is the dep_tree ${dep_tree}")

list(LENGTH dep_tree len)

debug_message(STATUS "------>  this is the length of the dep_tree list ${len}")


check_project_consistency(${project} ${version} "${dep_tree}" mismatch)

debug_message(STATUS "------>  this is the mismatch (should be 0): ${mismatch}")

debug_message(STATUS "======================================================================")


set(dep_tree "bla;3.2;toto;1.1;titi;2.1;tutu;4.9")

check_project_consistency(${project} ${version} "${dep_tree}" mismatch)

debug_message(STATUS "------>  this is the mismatch (should be 1): ${mismatch}")

debug_message(STATUS "======================================================================")

set(this_path "/home/hubert/Work/Space/Euclid" "/opt/euclid")

find_local_project(Elements 3.6 "${this_path}" proj_loc)

debug_message(STATUS "------>  this is the path to Elements : ${proj_loc}")


debug_message(STATUS "======================================================================")

debug_message(STATUS "===================${CMAKE_SOURCE_DIR}/CMakeLists.txt")

get_project_from_file(${CMAKE_SOURCE_DIR}/CMakeLists.txt project version dep_list)

debug_message(STATUS "project ${project} ${version} ${dep_list}")

debug_message(STATUS "===================${CMAKE_SOURCE_DIR}/InstallArea/x86_64-fc21-gcc49-dbg/BaseProjConfig.cmake")

