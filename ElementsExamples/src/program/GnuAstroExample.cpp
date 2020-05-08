/**
 * @file CCfitsExample.cpp
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

#include <gnuastro/fits.h>                    // header file to test
#include <gnuastro/cosmology.h>

#include "ElementsKernel/ProgramHeaders.h"  // for including all Program/related headers
#include "ElementsKernel/Unused.h"          // for ELEMENTS_UNUSED
#include "ElementsKernel/Auxiliary.h"

using std::string;
using std::map;

namespace Elements {
namespace Examples {

class GnuAstroExample: public Program {

public:

  ExitCode mainMethod(ELEMENTS_UNUSED map<string, VariableValue>& args) override {

    auto log = Logging::getLogger("GnuAstroExample");

    string test_upper_string {"THATSTRING"};
    log.info() << "This is the test upper string: " << test_upper_string;

    double z {2.5};
    double H0 {67.66};

    auto age = gal_cosmology_age(z, H0, 0.0, 0.0, 0.0);

    log.info() << "Age of the Universe @ z = " << z << " : " << age << " GA";


    return ExitCode::OK;

  }

};

}  // namespace Examples
}  // namespace Elements

/**
 * Implementation of a main using a base class macro
 * This must be present in all Elements programs
 */
MAIN_FOR(Elements::Examples::GnuAstroExample)
