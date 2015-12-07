/**
 * @file ProgramExample.cpp
 * @date January 6th, 2015
 * @author Pierre Dubath
 */

#include <cstdio>

#include <boost/program_options.hpp>        // for program options from configuration file of command line arguments
#include <boost/format.hpp>                 // for format

#include <gsl/gsl_sf_bessel.h>              // for gsl_sf_bessel_J0

#include "ElementsKernel/ProgramHeaders.h"  // for including all Program/related headers

namespace Elements {
namespace ElementsExamples {

namespace po = boost::program_options;

using namespace std;

class GslExample: public Elements::Program {

public:


  Elements::ExitCode mainMethod(map<string, po::variable_value>& /*args*/) override {

    Elements::Logging logger = Elements::Logging::getLogger("GslExample");


    double x = 5.0;
    double y = gsl_sf_bessel_J0 (x);


    logger.info() << boost::format("J0(%g) = %.18e\n") % x % y;

    return Elements::ExitCode::OK;

  }

};

} // namespace ElementsExamples
} // namespace Elements


/**
 * Implementation of a main using a base class macro
 * This must be present in all Elements programs
 */
MAIN_FOR(Elements::ElementsExamples::GslExample)

