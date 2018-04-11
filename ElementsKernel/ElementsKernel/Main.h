/**
 * @file ElementsKernel/Main.h
 *
 * @date Aug 27, 2014
 * @author Hubert Degaudenzi
 *
 * @copyright 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_MAIN_H_
#define ELEMENTSKERNEL_MAIN_H_

#include <exception>                               // for set_terminate
#include <memory>                                  // for unique_ptr

#include "ElementsKernel/Exit.h"                   // for ExitCode
#include "ElementsKernel/ProgramManager.h"         // for ProgramManager
#include "ElementsKernel/Export.h"                 // for ELEMENTS_API
#include "ElementsKernel/Unused.h"                 // for ELEMENTS_UNUSED

#include "ThisProject.h"
#include "ThisElementsModule.h"


/** @def MAIN_FOR(ELEMENTS_PROGRAM_NAME)
 * Macro which must be used to create a main in classes
 * that derived from Elements::Program, i.e., these derived classes
 * must end with the following line:
 * @code
 *    MAIN_FOR(ELEMENTS_PROGRAM_NAME)
 * @endcode.
 * * The macro is equiped with a set_terminate placed just before
 * the main function definition:
 * @code
 * ELEMENTS_UNUSED const auto installed{std::set_terminate(&Elements::ProgramManager::onTerminate)};
 * @endcode
 * That handler will do a smooth catching of any exception raised in the run part of the program and
 * call std::abort().
 * @param ELEMENTS_PROGRAM_NAME name of the main program class, derived from
 * the class Elements::Program class.
 */
#define MAIN_FOR(ELEMENTS_PROGRAM_NAME)         \
  ELEMENTS_UNUSED const auto installed={std::set_terminate(&Elements::ProgramManager::onTerminate)}; \
  ELEMENTS_API int main(int argc, char* argv[])              \
  {                                             \
    Elements::ProgramManager manager {std::unique_ptr<Elements::Program>{new ELEMENTS_PROGRAM_NAME{}}, \
                                      THIS_PROJECT_VERSION_STRING, THIS_PROJECT_NAME_STRING, \
                                      THIS_MODULE_VERSION_STRING, THIS_MODULE_NAME_STRING, \
                                      THIS_PROJECT_SEARCH_DIRS}; \
    Elements::ExitCode exit_code = manager.run(argc, argv);   \
    return static_cast<Elements::ExitCodeType>(exit_code);    \
  }

#endif // ELEMENTSKERNEL_MAIN_H_

/** \example ElementsExamples/src/program/ProgramExample.cpp
 * Example that shows the usage of the MAIN_FOR macro
 */

/**@}*/
