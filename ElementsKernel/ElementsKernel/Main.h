/**
 * @file Main.h
 *
 * @date Aug 27, 2014
 * @author Hubert Degaudenzi
 */

#ifndef ELEMENTSKERNEL_MAIN_H_
#define ELEMENTSKERNEL_MAIN_H_

#include <exception>                               // for set_terminate

#include "ElementsKernel/Exit.h"
#include "ElementsKernel/ProgramManager.h"
#include "ElementsKernel/Export.h"                 // for ELEMENTS_API
#include "ElementsKernel/Unused.h"                 // for ELEMENTS_UNUSED

#include "ThisProject.h"


/** @def MAIN_FOR(ELEMENTS_PROGRAM_NAME)
 * Macro which must be used to create a main in classes
 * that derived from Elements::Program, i.e., these derived classes
 * must end with the following line:
 * @code
 *    MAIN_FOR(ELEMENTS_PROGRAM_NAME)
 * @endcode
 * @param ELEMENTS_PROGRAM_NAME name of the main program class, derived from
 * the class Elements::Program class.
 */
#define MAIN_FOR(ELEMENTS_PROGRAM_NAME)         \
  ELEMENTS_UNUSED const auto installed{std::set_terminate(&Elements::ProgramManager::onTerminate)}; \
  ELEMENTS_API int main(int argc, char* argv[])              \
  {                                             \
    Elements::ProgramManager man {std::unique_ptr<Elements::Program>{new ELEMENTS_PROGRAM_NAME{}}, \
                                  THIS_PROJECT_VERSION_STRING, THIS_PROJECT_NAME_STRING}; \
    Elements::ExitCode exit_code =  man.run(argc, argv);   \
    return static_cast<Elements::ExitCodeType>(exit_code);    \
  }

#endif // ELEMENTSKERNEL_MAIN_H_


/** \example ElementsExamples/src/program/ProgramExample.cpp
 * Example that shows the usage of the MAIN_FOR macro
 */
