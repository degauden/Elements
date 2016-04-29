elements_subdir(ElementsKernel)

find_package(Boost REQUIRED COMPONENTS filesystem thread program_options system regex)
find_package(Log4CPP REQUIRED)

#---Libraries---------------------------------------------------------------
elements_add_library(ElementsKernel src/Lib/*.cpp src/Lib/*.h
                    LINK_LIBRARIES ${CMAKE_DL_LIBS} Boost Log4CPP
                    PUBLIC_HEADERS ElementsKernel)

#---Tests-------------------------------------------------------------------
elements_add_unit_test(Time tests/src/ElementsTime_test.cpp
                       EXECUTABLE ElementsTime_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(Timing tests/src/ElementsTiming_test.cpp
                       EXECUTABLE ElementsTiming_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(Memory tests/src/Memory_test.cpp
                       EXECUTABLE Memory_test
                       LINK_LIBRARIES ElementsKernel)
elements_add_unit_test(Real tests/src/Real_test.cpp
                       EXECUTABLE Real_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(Number tests/src/Number_test.cpp
                       EXECUTABLE Number_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
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
                       LINK_LIBRARIES ElementsKernel TYPE Boost)

#-----------------------
# Temporary_test
elements_add_unit_test(Temporary tests/src/Temporary_test.cpp
                       EXECUTABLE Temporary_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost)

#-----------------------
# ThisModule_test
elements_add_unit_test(ThisModule tests/src/ThisModule_test.cpp
                       EXECUTABLE ThisModule_test
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
                       LINK_LIBRARIES ElementsKernel TYPE Boost)

#-----------------------
# SystemOfUnits_test
elements_add_unit_test(SystemOfUnits tests/src/SystemOfUnits_test.cpp
                       EXECUTABLE SystemOfUnits_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost)


#-----------------------
# BackTrace_test
elements_add_unit_test(BackTrace tests/src/BackTrace_test.cpp
                       EXECUTABLE BackTrace_test
                       LINK_LIBRARIES ElementsKernel TYPE Boost)

#-----------------------
# GetEnv_test
elements_add_unit_test(GetEnv tests/src/GetEnv_test.cpp
                       EXECUTABLE GetEnv_test
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

elements_add_python_program(CreateElementsProject ElementsKernel.Project)
elements_add_python_program(AddElementsModule ElementsKernel.AddModule)
elements_add_python_program(AddCppClass ElementsKernel.AddCppClass)
elements_add_python_program(AddCppProgram ElementsKernel.AddCppProgram)
elements_add_python_program(AddPythonProgram ElementsKernel.AddPythonProgram)
elements_add_python_program(AddPythonModule ElementsKernel.AddPythonModule)
elements_add_python_program(RemoveCppClass ElementsKernel.RemoveCppClass)
elements_add_python_program(RemoveCppProgram ElementsKernel.RemoveCppProgram)
elements_add_python_program(RemovePythonProgram ElementsKernel.RemovePythonProgram)
elements_add_python_program(RemovePythonModule ElementsKernel.RemovePythonModule)

elements_install_aux_files()
elements_install_python_modules()

elements_add_test(CreateElementsProject COMMAND createElementsProject_test)
elements_add_test(AddElementsModule COMMAND addElementsModule_test)
elements_add_test(CreateCommonStructure COMMAND createCommonStructure_test)
elements_add_test(AddCppClass COMMAND addCppClass_test)
elements_add_test(AddCppProgram COMMAND addCppProgram_test)
elements_add_test(AddPythonModule COMMAND addPythonModule_test)
elements_add_test(AddPythonProgram COMMAND addPythonProgram_test)

