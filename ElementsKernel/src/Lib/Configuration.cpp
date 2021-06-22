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

#include <algorithm>  // for remove_if
#include <iterator>
#include <map>
#include <string>  // for string
#include <vector>  // for vector

#include <boost/filesystem/operations.hpp>  // for exists

#include "ElementsKernel/Path.h"    // for Path::VARIABLE, Path::Type
#include "ElementsKernel/System.h"  // for DEFAULT_INSTALL_PREFIX

using std::string;

namespace Elements {
inline namespace Kernel {

string getConfigurationVariableName() {
  return Path::VARIABLE.at(Path::Type::configuration);
}

// Instantiation of the most expected types
template Path::Item getConfigurationPath(const Path::Item& file_name, bool raise_exception);
template Path::Item getConfigurationPath(const string& file_name, bool raise_exception);

std::vector<Path::Item> getConfigurationLocations(bool exist_only) {

  auto location_list = Path::getLocations(Path::Type::configuration, exist_only);

  // the search is extended to the default system /usr/share/conf
  location_list.emplace_back(Path::Item(System::DEFAULT_INSTALL_PREFIX) / "share" / "conf");

  if (exist_only) {
    auto new_end = std::remove_if(location_list.begin(), location_list.end(), [](const Path::Item& p) {
      return (not boost::filesystem::exists(p));
    });
    location_list.erase(new_end, location_list.end());
  }

  return location_list;
}

namespace Configuration {

string getVariableName() {
  return getConfigurationVariableName();
}

// instantiation of the most expected types
template Path::Item getPath(const Path::Item& file_name, bool raise_exception);
template Path::Item getPath(const std::string& file_name, bool raise_exception);

std::vector<Path::Item> getLocations(bool exist_only) {
  return getConfigurationLocations(exist_only);
}

}  // namespace Configuration

}  // namespace Kernel
}  // namespace Elements
