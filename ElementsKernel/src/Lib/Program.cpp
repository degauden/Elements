/**
 * @file Program.cpp
 *
 * @date Jan 28, 2015
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
 *
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

  Program::~Program() {}

} // namespace Elements




