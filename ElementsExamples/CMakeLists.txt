# Declare ElementsExamples as an Elements module
elements_subdir(ElementsExamples)

# Declare Elements module dependencies
#    This module is using of the ElementsKernel module
elements_depends_on_subdirs(ElementsKernel)

# Declare external library dependencies
#    This module is using the Boost C++ library
#    CppUnit and GMock are optional dependencies
find_package(Boost REQUIRED COMPONENTS filesystem thread system program_options unit_test_framework)
find_package(CppUnit)
find_package(GMock)

#===== Libraries ===============================================================
# Instruction for creating a C++ library
#    ElementsExamples is the library name
#    src/Lib/*.cpp are the source files
elements_add_library(ElementsExamples src/lib/*.cpp
                    LINK_LIBRARIES Boost ElementsKernel
                    PUBLIC_HEADERS ElementsExamples)

#===== Executables =============================================================
# Instruction for creating a C++ executable
#    CppProgramExample is the executable name
#    src/Program/ProgramExample.cpp is the source file
elements_add_executable(CppProgramExample src/program/ProgramExample.cpp
                        INCLUDE_DIRS Boost ElementsExamples
                        LINK_LIBRARIES Boost ElementsExamples)

elements_add_executable(ElementsSimpleProgramExample src/program/SimpleProgramExample.cpp
                        INCLUDE_DIRS Boost ElementsExamples
                        LINK_LIBRARIES Boost ElementsExamples)

elements_add_executable(ElementsProgramExampleWithArguments src/program/ProgramExampleWithArguments.cpp
                        INCLUDE_DIRS Boost ElementsExamples
                        LINK_LIBRARIES Boost ElementsExamples)

# Example of a pure C program with a C++-compatible header file example.
elements_add_executable(CProgramExample src/CProgram/*.c)


#===== Test that the executables do run ========================================

elements_add_test(CppProgramExampleRun COMMAND CppProgramExample)
elements_add_test(CProgramExampleRun COMMAND CProgramExample)
elements_add_test(ElementsSimpleProgramExampleRun COMMAND ElementsSimpleProgramExample)
elements_add_test(ElementsProgramExampleWithArgumentsRun COMMAND ElementsProgramExampleWithArguments)




#===== Boost tests =============================================================
# Example with automatically generated TestMain.cpp. This is always the case if the TYPE of
# the test is Boost.
elements_add_unit_test(BoostClassExample tests/src/Boost/ClassExample_test.cpp
                       EXECUTABLE BoostClassExample_test
                       INCLUDE_DIRS ElementsExamples
                       LINK_LIBRARIES ElementsExamples TYPE Boost)

elements_add_unit_test(FunctionExample tests/src/Boost/FunctionExample_test.cpp
                       EXECUTABLE FunctionExample_test
                       INCLUDE_DIRS ElementsExamples
                       LINK_LIBRARIES ElementsExamples TYPE Boost)


# Example with explict TestMain.cpp. Please mind the "unit_test_framework" in the boost components
# from the find_package command. The Boost components have to be also explicitly added to the
# LINK_LIBRARIES argument.
elements_add_unit_test(BoostClassExampleWithMain tests/src/Boost/ClassExample_test.cpp tests/src/Boost/TestMain.cpp
                       EXECUTABLE BoostClassExampleWithMain_test
                       INCLUDE_DIRS ElementsExamples
                       LINK_LIBRARIES Boost ElementsExamples)

elements_add_unit_test(ModuleInfo tests/src/Boost/ModuleInfo_test.cpp
                       EXECUTABLE ElementsExamplesModuleInfo_test
                       INCLUDE_DIRS ElementsExamples
                       LINK_LIBRARIES ElementsExamples TYPE Boost)

elements_add_unit_test(AllTests tests/src/Boost/*_test.cpp
                       EXECUTABLE BoostElementsExampleAllTests
                       INCLUDE_DIRS ElementsExamples
                       LINK_LIBRARIES ElementsExamples TYPE Boost)

elements_add_unit_test(UnitTestExample_test tests/src/Boost/UnitTestExample_test.cpp
                       INCLUDE_DIRS ElementsExamples
                       LINK_LIBRARIES ElementsExamples TYPE Boost)


#===== CppUnit tests =============================================================

if(CPPUNIT_FOUND)
# CppUnit Type test
elements_add_unit_test(OtherClassExample tests/src/CppUnit/ClassExample_test.cpp
                       EXECUTABLE OtherClassExample_test
                       INCLUDE_DIRS ElementsExamples
                       LINK_LIBRARIES ElementsExamples TYPE CppUnit)

# Test with the CppUnit library. The TestMain.cpp is not generated
elements_add_unit_test(OtherClassExampleWithMain tests/src/CppUnit/ClassExample_test.cpp tests/src/CppUnit/TestMain.cpp
                       EXECUTABLE ElementsExamples_OtherClassExampleWithMain_test
                       LINK_LIBRARIES CppUnit ElementsExamples
                       INCLUDE_DIRS CppUnit ElementsExamples
                       LABELS CustomLabel CppUnit)
endif()

#===== Tests using GMock =======================================================
if(GMOCK_FOUND)

elements_add_unit_test(DataSourceUser_test_suite tests/src/GMock/DataSourceUser_test.cpp
                       LINK_LIBRARIES GMock ElementsExamples TYPE Boost
                       INCLUDE_DIRS GMock ElementsExamples
                       LABELS GMock)

elements_add_unit_test(TemplatedDataSourceUser_test_suite tests/src/GMock/TemplatedDataSourceUser_test.cpp
                       LINK_LIBRARIES GMock ElementsExamples TYPE Boost
                       INCLUDE_DIRS GMock ElementsExamples
                       LABELS GMock)

# inhibits the -Wsuggest-override that is not supported by gmock (yet)
if(CXX_HAS_SUGGEST_OVERRIDE)
  set_property(SOURCE tests/src/GMock/DataSourceUser_test.cpp tests/src/GMock/TemplatedDataSourceUser_test.cpp
               PROPERTY COMPILE_FLAGS -Wno-suggest-override)
endif() 
                
                       
endif(GMOCK_FOUND)

#====== Build Tests =================================================================


elements_add_executable(BackTraceExample src/program/BackTraceExample.cpp
                        INCLUDE_DIRS ElementsExamples
                        LINK_LIBRARIES ElementsExamples)
elements_add_test(BackTraceFailure COMMAND BackTraceExample FAILS)

find_package(GSL QUIET)
if(GSL_FOUND)
elements_add_executable(GslExample src/program/GslExample.cpp
                        INCLUDE_DIRS GSL ElementsExamples
                        LINK_LIBRARIES GSL ElementsExamples)
elements_add_test(GSLWorks COMMAND GslExample LABELS GSL Build)
endif()


find_package(HealpixCpp QUIET)
if(HEALPIXCPP_FOUND)
elements_add_executable(HealpixExample src/program/HealpixExample.cpp
                        INCLUDE_DIRS HealpixCpp ElementsExamples
                        LINK_LIBRARIES HealpixCpp ElementsExamples)
elements_add_test(HealpixCppWorks COMMAND HealpixExample LABELS Healpix Build)
endif()


find_package(XercesC QUIET)
if(XERCESC_FOUND)
elements_add_executable(XercesExample src/program/XercesExample.cpp
                        INCLUDE_DIRS XercesC ElementsExamples
                        LINK_LIBRARIES XercesC ElementsExamples)
elements_add_test(XercesCWorks COMMAND XercesExample LABELS Xerces Build)
endif()


find_package(WCSLIB QUIET)
if(WCSLIB_FOUND)
elements_add_executable(WcsExample src/program/WcsExample.cpp
                        INCLUDE_DIRS WCSLIB ElementsExamples
                        LINK_LIBRARIES WCSLIB ElementsExamples)
elements_add_test(WCSLIBWorks COMMAND WcsExample LABELS Wcs Build)
endif()


find_package(FFTW QUIET)
if(FFTW_FOUND)
elements_add_executable(FftwExample src/program/FftwExample.cpp
                        INCLUDE_DIRS FFTW ElementsExamples
                        LINK_LIBRARIES FFTW ElementsExamples)
elements_add_test(FFTWWorks COMMAND FftwExample LABELS Fftw Build)
endif()

find_package(Eigen3 QUIET)
if(EIGEN3_FOUND)
elements_add_executable(EigenExample src/program/EigenExample.cpp
                        LINK_LIBRARIES Eigen3 ElementsExamples
                        INCLUDE_DIRS Eigen3 ElementsExamples)
elements_add_test(EigenWorks COMMAND EigenExample LABELS Eigen Build)
endif()

find_package(CCfits QUIET)
if(CCFITS_FOUND)
elements_add_executable(CCfitsExample src/program/CCfitsExample.cpp
                        LINK_LIBRARIES CCfits ElementsExamples
                        INCLUDE_DIRS CCfits ElementsExamples)
elements_add_test(CCfitsWorks COMMAND CCfitsExample LABELS CCfits Build)
endif()

find_package(Cfitsio QUIET)
if(CFITSIO_FOUND AND CFITSIO_IS_REENTRANT)
elements_add_executable(CfitsioExample src/program/CfitsioExample.cpp
                        LINK_LIBRARIES Cfitsio ElementsExamples
                        INCLUDE_DIRS Cfitsio ElementsExamples)
elements_add_test(CfitsioWorks COMMAND CfitsioExample LABELS Cfitsio Build)
endif()

find_package(SWIG QUIET)
find_package(PythonLibs ${PYTHON_EXPLICIT_VERSION} QUIET)
if(SWIG_FOUND AND PYTHONLIBS_FOUND)
elements_add_swig_binding(SwigExample ElementsExamples/SwigExample.i
                          LINK_LIBRARIES ElementsExamples
                          INCLUDE_DIRS ElementsExamples
                          PUBLIC_HEADERS ElementsExamples)
endif()

#---Python modules, Scripts, conf and aux files ------------------------------------------------------------

elements_install_python_modules(TEST_TIMEOUT 30)

#===== Python programs =========================================================
# Instruction for creating a Python executable
#    PythonProgramExample is the executable name
#    ElementsExamples.PythonProgramExample is the source file
elements_add_python_program(PythonProgramExample ElementsExamples.PythonProgramExample)
if(SWIG_FOUND AND PYTHONLIBS_FOUND)
  elements_add_python_program(SwigProgramExample ElementsExamples.SwigProgramExample)
endif()
#===== Test that the python program does run ========================================
elements_add_test(PythonProgramExampleRun COMMAND PythonProgramExample)
if(SWIG_FOUND AND PYTHONLIBS_FOUND)
elements_add_test(SwigProgramExampleRun COMMAND SwigProgramExample LABELS Swig Build)
endif()

elements_install_scripts()


elements_add_test(ScriptThatSucceeds COMMAND ScriptThatSucceeds_test LABELS CustomLabel)
elements_add_test(ScriptThatChecksFile COMMAND ScriptThatChecksFile_test)

elements_add_test(ScriptThatFails COMMAND ScriptThatFails_test FAILS)
elements_add_test(ScriptThatGivesOutput COMMAND ScriptThatGivesOutput_test PASSREGEX ".*good")
elements_add_test(ScriptThatGivesError COMMAND ScriptThatGivesError_test FAILREGEX ".*bad" FAILS)
elements_add_test(ScriptThatGivesStdError COMMAND ScriptThatGivesStdError_test FAILREGEX ".*worse" FAILS)

elements_add_test(CppProgramExample COMMAND CppProgramExample)
elements_add_test(SimpleProgramExample COMMAND ElementsSimpleProgramExample)

elements_install_conf_files()

