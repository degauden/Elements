CMAKE_MINIMUM_REQUIRED(VERSION 2.8.5)
#===============================================================================
# Load elements_subdir macro here
# Examples:
#   For declaring a project module:
#         elements_subdir(ElementsExamples)
#===============================================================================
elements_subdir(ElementsServices)

find_package(Irods QUIET)
find_package(Wget QUIET)
find_python_module(easdss)

#===============================================================================
# Load elements_depends_on_subdirs macro here
#   For creating a dependency onto an other accessible module
#         elements_depends_on_subdirs(ElementsKernel)
#===============================================================================
elements_depends_on_subdirs(ElementsKernel)

#===============================================================================
# Add the find_package macro (a pure CMake command) here to locate the
# libraries.
# Examples:
#          find_package(CppUnit)
#===============================================================================

#===============================================================================
# Declare the library dependencies here
# Example:
#         elements_add_library(ElementsExamples src/Lib/*.cpp
#                     LINK_LIBRARIES Boost ElementsKernel
#                     INCLUDE_DIRS Boost ElementsKernel
#                     PUBLIC_HEADERS ElementsExamples)
#===============================================================================
elements_add_library(ElementsServices src/lib/DataSync/*.cpp src/lib/*.cpp
                     LINK_LIBRARIES ElementsKernel
                     INCLUDE_DIRS ElementsKernel
                     PUBLIC_HEADERS ElementsServices)

#===============================================================================
# Declare the executables here
# Example:
# elements_add_executable(ElementsProgramExample src/Program/ProgramExample.cpp
#                        LINK_LIBRARIES Boost ElementsExamples
#                        INCLUDE_DIRS Boost ElementsExamples)
#===============================================================================

#===============================================================================
# Declare the Boost tests here
# Example:
# elements_add_unit_test(BoostClassExample tests/src/Boost/ClassExample_test.cpp
#                       EXECUTABLE BoostClassExample_test
#                       INCLUDE_DIRS ElementsExamples
#                       LINK_LIBRARIES ElementsExamples TYPE Boost)
#===============================================================================
elements_add_unit_test(DataSync tests/src/DataSync_test.cpp tests/src/DataSync/fixtures/*.cpp
                       EXECUTABLE ElementsServices_DataSync_test
                       LINK_LIBRARIES ElementsServices
                       ENVIRONMENT BOOST_TEST_CATCH_SYSTEM_ERRORS=no
                       TYPE Boost)
elements_add_unit_test(ConnectionConfiguration tests/src/DataSync/ConnectionConfiguration_test.cpp tests/src/DataSync/fixtures/*.cpp
                       EXECUTABLE ElementsServices_ConnectionConfiguration_test
                       LINK_LIBRARIES ElementsServices
                       TYPE Boost)
elements_add_unit_test(DataSynchronizer tests/src/DataSync/DataSynchronizer_test.cpp tests/src/DataSync/fixtures/*.cpp
                       EXECUTABLE ElementsServices_DataSynchronizer_test
                       LINK_LIBRARIES ElementsServices
                       TYPE Boost)
elements_add_unit_test(DataSynchronizerMaker tests/src/DataSync/DataSynchronizerMaker_test.cpp 
                       EXECUTABLE ElementsServices_DataSynchronizerMaker_test
                       LINK_LIBRARIES ElementsServices
                       TYPE Boost)
elements_add_unit_test(DataSyncUtils tests/src/DataSync/DataSyncUtils_test.cpp 
                       EXECUTABLE ElementsServices_DataSyncUtils_test
                       LINK_LIBRARIES ElementsServices
                       ENVIRONMENT BOOST_TEST_CATCH_SYSTEM_ERRORS=no WORKSPACE=/home/user/Work/Projects
                       TYPE Boost)
elements_add_unit_test(DependencyConfiguration tests/src/DataSync/DependencyConfiguration_test.cpp tests/src/DataSync/fixtures/*.cpp
                       EXECUTABLE ElementsServices_DependencyConfiguration_test
                       LINK_LIBRARIES ElementsServices
                       TYPE Boost)
if(PY_EASDSS_FOUND)
elements_add_unit_test(DssSynchronizer tests/src/DataSync/DssSynchronizer_test.cpp tests/src/DataSync/fixtures/*.cpp
                       EXECUTABLE ElementsServices_DssSynchronizer_test
                       LINK_LIBRARIES ElementsServices
                       TYPE Boost)
endif()     
if(IRODS_FOUND)
  elements_add_unit_test(IrodsSynchronizer tests/src/DataSync/IrodsSynchronizer_test.cpp tests/src/DataSync/fixtures/*.cpp
                         EXECUTABLE ElementsServices_IrodsSynchronizer_test
                         LINK_LIBRARIES ElementsServices
                         ENVIRONMENT BOOST_TEST_CATCH_SYSTEM_ERRORS=no
                         TYPE Boost)
endif()

if(WGET_FOUND)
  elements_add_unit_test(WebdavSynchronizer tests/src/DataSync/WebdavSynchronizer_test.cpp tests/src/DataSync/fixtures/*.cpp
                         EXECUTABLE ElementsServices_WebdavSynchronizer_test
                         LINK_LIBRARIES ElementsServices
                         TYPE Boost)
endif()
#===============================================================================
# Use the following macro for python modules, scripts and aux files:
#  elements_install_python_modules()
#  elements_install_scripts()
#===============================================================================
elements_install_python_modules()

#===============================================================================
# Declare the Python programs here
# Examples :
# elements_add_python_program(PythonProgramExample
#                             ElementsExamples.PythonProgramExample)
#===============================================================================

#===============================================================================
# Add the elements_install_conf_files macro
# Examples:
#          elements_install_conf_files()
#===============================================================================
