/**
 * @file Auxiliary.cpp
 *
 * @date Feb 8, 2017
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
 *
 */

#include "ElementsKernel/Auxiliary.h"

#include <algorithm>                   // for remove_if
#include <iterator>
#include <map>
#include <string>                      // for string
#include <vector>                      // for vector

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

#include "ElementsKernel/Path.h"       // for Type and VARIABLE
#include "ElementsKernel/System.h"     // for DEFAULT_INSTALL_PREFIX

using std::string;
using std::vector;
using boost::filesystem::path;
using Elements::Path::Type;
using Elements::Path::VARIABLE;

namespace Elements {

string getAuxiliaryVariableName() {
  return VARIABLE.at(Type::auxiliary);
}

// instantiation of the most expected types
template path getAuxiliaryPath(const path& file_name, bool raise_exception);
template path getAuxiliaryPath(const string& file_name, bool raise_exception);

vector<path> getAuxiliaryLocations(bool exist_only) {

  using System::DEFAULT_INSTALL_PREFIX;

  auto location_list = Path::getLocationsFromEnv(VARIABLE.at(Type::auxiliary), exist_only);

  // extended to /usr/share/aux{dir,}
  location_list.push_back(path(DEFAULT_INSTALL_PREFIX) / "share" / "auxdir");
  // for backward compatibility with the former convention
  location_list.push_back(path(DEFAULT_INSTALL_PREFIX) / "share" / "aux");

  if (exist_only) {
    auto new_end = std::remove_if(location_list.begin(),
                                  location_list.end(),
                                  [](path p){
                                     return boost::filesystem::exists(p);
                                  });
    location_list.erase(new_end, location_list.end());
  }

  return location_list;
}

}  // namespace Elements
