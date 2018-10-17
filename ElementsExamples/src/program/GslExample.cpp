/**
 * @file GslExample.cpp
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

#include <map>                              // for map
#include <string>                           // for string
#include <boost/program_options.hpp>        // for program options from configuration file of command line arguments
#include <boost/format.hpp>                 // for format

#include <gsl/gsl_version.h>
#include <gsl/gsl_sf_bessel.h>              // for gsl_sf_bessel_J0

#include "ElementsKernel/ProgramHeaders.h"  // for including all Program/related headers
#include "ElementsKernel/Unused.h"          // for ELEMENTS_UNUSED

using std::map;
using std::string;
using boost::program_options::variable_value;

namespace Elements {
namespace Examples {

class GslExample: public Program {

public:

  ExitCode mainMethod(ELEMENTS_UNUSED map<string, variable_value>& args) override {

    auto log = Logging::getLogger("GslExample");

    log.info() << "GSL version: " << gsl_version;

    double x = 5.0;
    double y = gsl_sf_bessel_J0(x);

    log.info() << boost::format("J0(%g) = %.18e\n") % x % y;

    return ExitCode::OK;

  }

};

}  // namespace Examples
}  // namespace Elements


/**
 * Implementation of a main using a base class macro
 * This must be present in all Elements programs
 */
MAIN_FOR(Elements::Examples::GslExample)
