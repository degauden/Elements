/**
 * @file WcsExample.cpp
 * @date January 6th, 2015
 * @author Pierre Dubath
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
 *
 */

#include <map>                                 // for map
#include <string>                              // for string
#include <boost/program_options.hpp>           // for program options from configuration file of command line arguments
#include <stdexcept>                           // for standard exceptions

#include "ElementsExamples/crashingFunction.h" // for crashingFunction

#include "ElementsKernel/ProgramHeaders.h"     // for including all Program/related headers
#include "ElementsKernel/Unused.h"             // for ELEMENTS_UNUSED

namespace Elements {
namespace Examples {

namespace po = boost::program_options;

using std::map;
using std::string;

void secondLevelFunction() {
  Logging logger = Logging::getLogger("BackTraceExample");
  logger.info() << "Entering Second Level Function";

  crashingFunction();
}

void firstLevelFunction() {
  Logging logger = Logging::getLogger("BackTraceExample");
  logger.info() << "Entering First Level Function";

  secondLevelFunction();
}

class BackTraceExample: public Program {

public:

  ExitCode mainMethod(ELEMENTS_UNUSED map<string, po::variable_value>& args) override {

    Logging logger = Logging::getLogger("BackTraceExample");

    firstLevelFunction();

    logger.info() << "done with test program! ";

    return ExitCode::OK;

  }

};

} // namespace ElementsExamples
} // namespace Elements


/**
 * Implementation of a main using a base class macro
 * This must be present in all Elements programs
 */
MAIN_FOR(Elements::Examples::BackTraceExample)
