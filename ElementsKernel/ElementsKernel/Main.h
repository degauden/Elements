/**
 * @file Main.h
 *
 * @date Aug 27, 2014
 * @author Hubert Degaudenzi
 */

#ifndef ELELMENTSKERNEL_MAIN_H_
#define ELELMENTSKERNEL_MAIN_H_

#include "ElementsKernel/Exit.h"

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
  int main(int argc, char* argv[])                  \
  {                                                 \
    ELEMENTS_PROGRAM_NAME elementProgramInstance {} ; \
    Elements::ExitCode exit_code = elementProgramInstance.run(argc, argv) ;       \
    return static_cast<Elements::ExitCodeType>(exit_code) ;                                       \
  }





#endif // ELELMENTSKERNEL_MAIN_H_
