/**
 * @file Program.cpp
 *
 * @date Jan 28, 2015
 * @author hubert
 */

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "ElementsKernel/Program.h"

using namespace std;


namespace Elements {

  po::options_description Program::defineSpecificProgramOptions() {
    po::options_description config_desc("");
    return config_desc;
  }


} // namespace Elements




