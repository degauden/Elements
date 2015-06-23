elements_subdir(ElementsExamples)

elements_depends_on_subdirs(ElementsKernel)


find_package(Boost REQUIRED COMPONENTS filesystem thread system program_options unit_test_framework)
find_package(CppUnit)
find_package(GMock)

#===== Libraries ===============================================================
elements_add_library(ElementsExamples src/Lib/*.cpp
                    LINK_LIBRARIES Boost ElementsKernel
                    INCLUDE_DIRS Boost ElementsKernel
                    PUBLIC_HEADERS ElementsExamples)

#===== Executables =============================================================
elements_add_executable(ElementsProgramExample src/Program/ProgramExample.cpp
                        LINK_LIBRARIES Boost ElementsExamples
                        INCLUDE_DIRS Boost ElementsExamples)

elements_add_executable(ElementsSimpleProgramExample src/Program/SimpleProgramExample.cpp
                        LINK_LIBRARIES Boost ElementsExamples
                        INCLUDE_DIRS Boost ElementsExamples)

elements_add_executable(ElementsProgramExampleWithArguments src/Program/ProgramExampleWithArguments.cpp
                        LINK_LIBRARIES Boost ElementsExamples
                        INCLUDE_DIRS Boost ElementsExamples)


#===== Python programs =========================================================
elements_add_python_program(PythonProgramExample ElementsExamples.PythonProgramExample)

#===== Boost tests =============================================================
# Example with automatically generated TestMain.cpp. This is always the case if the TYPE of
# the test is Boost.
elements_add_unit_test(BoostClassExample tests/src/Boost/ClassExample_test.cpp
                       EXECUTABLE BoostClassExample_test
                       INCLUDE_DIRS ElementsExamples
                       LINK_LIBRARIES ElementsExamples TYPE Boost)

# Example with explict TestMain.cpp. Please mind the "unit_test_framework" in the boost components
# from the find_package command. The Boost components have to be also explicitly added to the
# LINK_LIBRARIES argument.
elements_add_unit_test(BoostClassExampleWithMain tests/src/Boost/ClassExample_test.cpp tests/src/Boost/TestMain.cpp
                       EXECUTABLE BoostClassExampleWithMain_test
                       INCLUDE_DIRS Boost ElementsExamples
                       LINK_LIBRARIES Boost ElementsExamples)

elements_add_unit_test(ModuleInfo tests/src/Boost/ModuleInfo_test.cpp
                       EXECUTABLE ElementsExamplesModuleInfo_test
                       INCLUDE_DIRS ElementsExamples
                       LINK_LIBRARIES ElementsExamples TYPE Boost)


elements_add_unit_test(AllTests tests/src/Boost/*_test.cpp
                       EXECUTABLE BoostElementsExampleAllTests
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
                       EXECUTABLE OtherClassExampleWithMain_test
                       INCLUDE_DIRS CppUnit ElementsExamples
                       LINK_LIBRARIES CppUnit ElementsExamples
                       LABELS CustomLabel)
endif()

#===== Tests using GMock =======================================================

elements_add_unit_test(ExampleClass1 tests/src/GMock/ExampleClass1_test.cpp
                       EXECUTABLE ExampleClass1_test
                       LINK_LIBRARIES ElementsExamples TYPE Boost)

if(GMOCK_FOUND)
  elements_add_unit_test(ExampleClass2 tests/src/GMock/ExampleClass2_test.cpp
                         EXECUTABLE ExampleClass2_test
                         LINK_LIBRARIES GMock ElementsExamples TYPE Boost
                         INCLUDE_DIRS GMock ElementsExamples
                         LABELS GMock)
endif()

#---Python modules, Scripts, conf and aux files ------------------------------------------------------------

elements_install_conf_files()

elements_install_python_modules()

elements_install_scripts()


elements_add_test(ScriptThatSucceeds COMMAND ScriptThatSucceeds_test LABELS CustomLabel)
elements_add_test(ScriptThatFails COMMAND ScriptThatFails_test FAILS)
elements_add_test(ScriptThatGivesOutput COMMAND ScriptThatGivesOutput_test PASSREGEX ".*good")
elements_add_test(ScriptThatGivesError COMMAND ScriptThatGivesError_test FAILREGEX ".*bad" FAILS)
elements_add_test(ScriptThatGivesStdError COMMAND ScriptThatGivesStdError_test FAILREGEX ".*worse" FAILS)

elements_add_test(ProgramExample COMMAND ElementsProgramExample)
elements_add_test(SimpleProgramExample COMMAND ElementsSimpleProgramExample)

