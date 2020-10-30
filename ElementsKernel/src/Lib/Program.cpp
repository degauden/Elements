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

#include "ElementsKernel/Program.h"

#include <utility>  // for pair

namespace Elements {

using OptionsDescription           = Program::OptionsDescription;
using PositionalOptionsDescription = Program::PositionalOptionsDescription;

OptionsDescription Program::defineSpecificProgramOptions() {
  OptionsDescription config_desc("");
  return config_desc;
}

std::pair<OptionsDescription, PositionalOptionsDescription> Program::defineProgramArguments() {
  OptionsDescription           desc("");
  PositionalOptionsDescription pos_desc;

  return std::make_pair(desc, pos_desc);
}

Program::~Program() = default;

}  // namespace Elements
