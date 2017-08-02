/**
 * @file Configuration.cpp
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

#include "ElementsKernel/Configuration.h"

#include <string>                         // for string
#include <vector>                         // for vector
#include <algorithm>                      // for remove_if
#include <boost/filesystem.hpp>           // for boost::filesystem

#include "ElementsKernel/System.h"        // for DEFAULT_INSTALL_PREFIX
#include "ElementsKernel/Path.h"          // for Path::VARIABLE, Path::Type
                                           // for Path::getLocationsFromEnv
using std::string;
using std::vector;
using boost::filesystem::path;

namespace Elements {


string getConfigurationVariableName() {
  return Path::VARIABLE.at(Path::Type::configuration);
}

// Instantiation of the most expected types
template path getConfigurationPath(const path& file_name, bool raise_exception);
template path getConfigurationPath(const string& file_name, bool raise_exception);

vector<path> getConfigurationLocations(bool exist_only) {

  auto location_list = Path::getLocationsFromEnv(Path::VARIABLE.at(Path::Type::configuration), exist_only);

  // the search is extended to the default system /usr/share/conf
  location_list.push_back(path(System::DEFAULT_INSTALL_PREFIX) / "share" / "conf");

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
