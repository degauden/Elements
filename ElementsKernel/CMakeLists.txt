elements_subdir(ElementsKernel)

find_package(Boost REQUIRED COMPONENTS filesystem thread program_options system regex)
find_package(Log4CPP)

#---Libraries---------------------------------------------------------------
elements_add_library(ElementsKernel src/Lib/*.cpp
                    LINK_LIBRARIES ${CMAKE_DL_LIBS} Boost Log4CPP
                    INCLUDE_DIRS Boost Log4CPP
                    PUBLIC_HEADERS ElementsKernel)

#---Tests-------------------------------------------------------------------
elements_add_unit_test(Time tests/src/ElementsTime_test.cpp
                       EXECUTABLE ElementsTime_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(Timing tests/src/ElementsTiming_test.cpp
                       EXECUTABLE ElementsTiming_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(Memory tests/src/Memory_test.cpp
                       EXECUTABLE Memory_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel)
elements_add_unit_test(Real tests/src/Real_test.cpp
                       EXECUTABLE Real_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(Number tests/src/Number_test.cpp
                       EXECUTABLE Number_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(Version tests/src/Version_test.cpp
                       EXECUTABLE Version_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
elements_add_unit_test(Logging tests/src/ElementsLogging_test.cpp
                       EXECUTABLE ElementsLogging_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)
#-----------------------
# Path_test
elements_add_unit_test(PathSearch tests/src/PathSearch_test.cpp
                       EXECUTABLE PathSearch_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)

#-----------------------
# Temporary_test
elements_add_unit_test(Temporary tests/src/Temporary_test.cpp
                       EXECUTABLE Temporary_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)

#-----------------------
# ThisModule_test
elements_add_unit_test(ThisModule tests/src/ThisModule_test.cpp
                       EXECUTABLE ThisModule_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)

#-----------------------
# Storage_test
elements_add_unit_test(Storage tests/src/Storage_test.cpp
                       EXECUTABLE Storage_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)


#-----------------------
# MathConstants_test
elements_add_unit_test(MathConstants tests/src/MathConstants_test.cpp
                       EXECUTABLE MathConstants_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)

#-----------------------
# SystemOfUnits_test
elements_add_unit_test(SystemOfUnits tests/src/SystemOfUnits_test.cpp
                       EXECUTABLE SystemOfUnits_test
                       INCLUDE_DIRS ElementsKernel
                       LINK_LIBRARIES ElementsKernel TYPE Boost)


#-----------------------
# SystemOfUnits_test
elements_add_unit_test(BackTrace tests/src/BackTrace_test.cpp
                       EXECUTABLE BackTrace_test
                       INCLUDE_DIRS ElementsKernel
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

elements_add_python_program(CreateElementsProject ElementsKernel.ElementsProject)
elements_add_python_program(AddElementsModule ElementsKernel.ElementsAddModule)
elements_add_python_program(AddCppClass ElementsKernel.ElementsAddCppClass)
elements_add_python_program(AddCppProgram ElementsKernel.ElementsAddCppProgram)
elements_add_python_program(AddPythonProgram ElementsKernel.ElementsAddPythonProgram)
elements_add_python_program(AddPythonModule ElementsKernel.ElementsAddPythonModule)

elements_install_aux_files()
elements_install_python_modules()

elements_add_test(CreateProject COMMAND createElementsProject_test)
elements_add_test(CreateModule COMMAND createElementsModule_test)
