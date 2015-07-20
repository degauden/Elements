/**
 * @file Program.cpp
 *
 * @date Jan 28, 2015
 * @author hubert
 */

#include <utility>

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include "ElementsKernel/Program.h"

using std::pair;

namespace Elements {

  po::options_description Program::defineSpecificProgramOptions() {
    po::options_description config_desc("");
    return config_desc;
  }

  pair<po::options_description,po::positional_options_description> Program::defineProgramArguments() {
    po::options_description desc("");
    po::positional_options_description pos_desc;

    using std::make_pair;

    return make_pair(desc, pos_desc);
  }


} // namespace Elements




