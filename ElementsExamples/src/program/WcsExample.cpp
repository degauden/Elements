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

#include <cstdio>

#include <boost/program_options.hpp>        // for program options from configuration file of command line arguments
#include <boost/format.hpp>                 // for format

#include <wcslib/wcs.h>

#include "ElementsKernel/ProgramHeaders.h"  // for including all Program/related headers
#include "ElementsKernel/Unused.h"          // for ELEMENTS_UNUSED

namespace Elements {
namespace Examples {

namespace po = boost::program_options;

using namespace std;

class WcsExample: public Elements::Program {

public:


  Elements::ExitCode mainMethod(ELEMENTS_UNUSED map<string, po::variable_value>& args) override {

    Elements::Logging logger = Elements::Logging::getLogger("WcsExample");

    logger.info() << "done with test program! ";

    return Elements::ExitCode::OK;

  }

};

} // namespace ElementsExamples
} // namespace Elements


/**
 * Implementation of a main using a base class macro
 * This must be present in all Elements programs
 */
MAIN_FOR(Elements::Examples::WcsExample)

