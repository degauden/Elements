/**
 * @file Main.h
 *
 * @date Aug 27, 2014
 * @author Hubert Degaudenzi
 */

#ifndef ELEMENTSKERNEL_MAIN_H_
#define ELEMENTSKERNEL_MAIN_H_

#include "ElementsKernel/Exit.h"
#include "ElementsKernel/ProgramManager.h"
#include "ThisProject.h"

/**
 * Macro which must be used to create a main in classes
 * that derived from Elements::Program, i.e., these derived classes
 * must end with the following line:
 *
 *    MAIN_FOR(ELEMENTS_PROGRAM_NAME)
 *
 *  ElementsProgramExample.cpp shows how to use this macro
 */
#define MAIN_FOR(ELEMENTS_PROGRAM_NAME)         \
  int main(int argc, char* argv[])              \
  {                                             \
    Elements::ProgramManager man {std::unique_ptr<Elements::Program>{new ELEMENTS_PROGRAM_NAME{}}, \
                                  THIS_PROJECT_VERSION_STRING, THIS_PROJECT_NAME_STRING}; \
    Elements::ExitCode exit_code =  man.run(argc, argv);   \
    return static_cast<Elements::ExitCodeType>(exit_code);    \
  }

#endif // ELEMENTSKERNEL_MAIN_H_
