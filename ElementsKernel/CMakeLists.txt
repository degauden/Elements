elements_subdir(ElementsKernel)

find_package(Boost REQUIRED COMPONENTS filesystem thread program_options system regex)
find_package(Log4CPP REQUIRED)

#---Libraries---------------------------------------------------------------
elements_add_library(ElementsKernel src/Lib/*.cpp
                    LINK_LIBRARIES ${CMAKE_DL_LIBS} Boost Log4CPP
                    INCLUDE_DIRS Boost Log4CPP
                    PUBLIC_HEADERS ElementsKernel)

#---Tests-------------------------------------------------------------------
elements_add_unit_test(Time tests/src/ElementsTime_test.cpp
                       EXECUTABLE ElementsTime_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(Real tests/src/Real_test.cpp
                       EXECUTABLE Real_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost
                       LABELS Math)
elements_add_unit_test(Number tests/src/Number_test.cpp
                       EXECUTABLE Number_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost
                       LABELS Math)
elements_add_unit_test(Version tests/src/Version_test.cpp
                       EXECUTABLE Version_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(Logging tests/src/ElementsLogging_test.cpp
                       EXECUTABLE ElementsLogging_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
#-----------------------
# Path_test
elements_add_unit_test(PathSearch tests/src/PathSearch_test.cpp
                       EXECUTABLE PathSearch_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost 
                       LABELS Path)

elements_add_unit_test(Path tests/src/Path_test.cpp
                       EXECUTABLE Path_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost 
                       LABELS Path)

elements_add_unit_test(Configuration tests/src/Configuration_test.cpp
                       EXECUTABLE BasicConfiguration_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost 
                       LABELS Path)

elements_add_unit_test(Auxiliary tests/src/Auxiliary_test.cpp
                       EXECUTABLE Auxiliary_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost 
                       LABELS Path)

#-----------------------
# Temporary_test
elements_add_unit_test(Temporary tests/src/Temporary_test.cpp
                       EXECUTABLE Temporary_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost)

#-----------------------
# Environment_test
elements_add_unit_test(Environment tests/src/Environment_test.cpp
                       EXECUTABLE Environment_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost
                       LABELS Environment)

#-----------------------
# ModuleInfo_test
elements_add_unit_test(ModuleInfo tests/src/ModuleInfo_test.cpp
                       EXECUTABLE ModuleInfo_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost)



#-----------------------
# ThisModule_test
# Test for the BINARY module (a library or an executable)
elements_add_unit_test(ThisModule tests/src/ThisModule_test.cpp
                       EXECUTABLE ThisModule_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost)

#-----------------------
# ThisProject_test
elements_add_unit_test(ThisProject tests/src/ThisProject_test.cpp
                       EXECUTABLE ThisProject_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost)

#-----------------------
# ThisElementsModule_test
# Test for the Elements Module (ElementsKernel for example)
elements_add_unit_test(ThisElementsModule tests/src/ThisElementsModule_test.cpp
                       EXECUTABLE ThisElementsModule_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost)


#-----------------------
# Storage_test
elements_add_unit_test(Storage tests/src/Storage_test.cpp
                       EXECUTABLE Storage_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost)


#-----------------------
# MathConstants_test
elements_add_unit_test(MathConstants tests/src/MathConstants_test.cpp
                       EXECUTABLE MathConstants_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost
                       LABELS Math)

#-----------------------
# SystemOfUnits_test
elements_add_unit_test(SystemOfUnits tests/src/SystemOfUnits_test.cpp
                       EXECUTABLE SystemOfUnits_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost
                       LABELS Math)


#-----------------------
# BackTrace_test
elements_add_unit_test(BackTrace tests/src/BackTrace_test.cpp
                       EXECUTABLE BackTrace_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost)

#-----------------------
# GetEnv_test
elements_add_unit_test(GetEnv tests/src/GetEnv_test.cpp
                       EXECUTABLE GetEnv_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost
                       LABELS Environment)

#-----------------------
# Sleep_test
elements_add_unit_test(Sleep tests/src/Sleep_test.cpp
                       EXECUTABLE Sleep_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost)

#-----------------------
# Exception_test
elements_add_unit_test(Exception tests/src/Exception_test.cpp
                       EXECUTABLE Exception_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost)


#if(ELEMENTS_HIDE_WARNINGS)
#  if(UNIX)
    # This hides warnings from a broken header file in CppUnit, but may
    # hide problems in the actual test source
#    set_property(SOURCE tests/src/ElementsTime_test.cpp
#                 PROPERTY COMPILE_FLAGS -Wno-unused-parameter)
#  endif()
#endif()

#---Python and Scripts ------------------------------------------------------------

elements_install_python_modules()


elements_add_python_program(CreateElementsProject ElementsKernel.Project)
elements_add_python_program(AddElementsModule ElementsKernel.AddModule)
elements_add_python_program(AddCppClass ElementsKernel.AddCppClass)
elements_add_python_program(AddCppProgram ElementsKernel.AddCppProgram)
elements_add_python_program(AddPythonProgram ElementsKernel.AddPythonProgram)
elements_add_python_program(AddScript ElementsKernel.AddScript)
elements_add_python_program(AddPythonModule ElementsKernel.AddPythonModule)
elements_add_python_program(RemoveCppClass ElementsKernel.RemoveCppClass)
elements_add_python_program(RemoveCppProgram ElementsKernel.RemoveCppProgram)
elements_add_python_program(RemovePythonProgram ElementsKernel.RemovePythonProgram)
elements_add_python_program(RemovePythonModule ElementsKernel.RemovePythonModule)
elements_add_python_program(ElementsNameCheck ElementsKernel.NameCheck)
elements_add_python_program(GetElementsFiles ElementsKernel.GetElementsFiles)

elements_install_aux_files()

elements_add_test(CreateElementsProject COMMAND createElementsProject_test LABELS Creation)
elements_add_test(AddElementsModule COMMAND addElementsModule_test LABELS Creation)
elements_add_test(CreateCommonStructure COMMAND createCommonStructure_test LABELS Creation)
elements_add_test(AddCppClass COMMAND addCppClass_test LABELS Creation)
elements_add_test(AddCppProgram COMMAND addCppProgram_test LABELS Creation)
elements_add_test(AddPythonModule COMMAND addPythonModule_test LABELS Creation)
elements_add_test(AddPythonProgram COMMAND addPythonProgram_test LABELS Creation)
elements_add_test(AddScript COMMAND addScript_test LABELS Creation)

elements_add_test(CreateDependentElementsProjects 
                  COMMAND createDependentElementsProjects_test  
                  ENVIRONMENT CMAKE_PREFIX_PATH+=${CMAKE_SOURCE_DIR}/cmake
                  LABELS Creation)

elements_add_test(CheckElementsProjectsTree
                  COMMAND checkElementsProjectsTree_test  
                  ENVIRONMENT CMAKE_PREFIX_PATH+=${CMAKE_SOURCE_DIR}/cmake
                  LABELS Creation)

elements_add_test(CheckProjectDependencyFailure
                  COMMAND checkProjectDependencyFailure_test  
                  ENVIRONMENT CMAKE_PREFIX_PATH+=${CMAKE_SOURCE_DIR}/cmake
                  LABELS Creation)

elements_add_test(HEADVersionProject
                  COMMAND HEADVersionProject_test
                  ENVIRONMENT CMAKE_PREFIX_PATH+=${CMAKE_SOURCE_DIR}/cmake
                  LABELS Creation)
                  
elements_add_test(PatchVersionProject
                  COMMAND patchVersionProject_test
                  ENVIRONMENT CMAKE_PREFIX_PATH+=${CMAKE_SOURCE_DIR}/cmake
                  LABELS Creation)
                  
