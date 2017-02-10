/**
 * @file Path.cpp
 *
 * @date May 13, 2016
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

#include "ElementsKernel/Path.h"

#include <string>                      // for string
#include <vector>                      // for vector
#include <algorithm>                   // for transform, remove_if
#include <map>                         // for map

#include <boost/filesystem.hpp>        // for boost::filesystem
#include <boost/algorithm/string.hpp>  // for boost::split

#include "ElementsKernel/System.h"     // for getEnv, SHLIB_VAR_NAME

using std::string;
using std::vector;
using std::map;

using boost::filesystem::path;

namespace Elements {
namespace Path {

const string PATH_SEP {":"};

const map<Type, const string> VARIABLE  {
  {Type::executable,                  "PATH"},
  {Type::library,     System::SHLIB_VAR_NAME},
  {Type::python,                "PYTHONPATH"},
  {Type::configuration, "ELEMENTS_CONF_PATH"},
  {Type::auxiliary,      "ELEMENTS_AUX_PATH"}
};

const map<Type, const vector<string>> SUFFIXES {
  {Type::executable, {"scripts", "bin"}},
  {Type::library, {"lib"}},
  {Type::python, {"python"}},
  {Type::configuration, {"conf"}},
  {Type::auxiliary, {"auxdir", "aux"}}
};

const map<Type, const vector<string>> DEFAULT_LOCATIONS {
  {Type::executable, {}},
  {Type::library, {"/usr/lib64", "/usr/lib"}},
  {Type::python, {}},
  {Type::configuration, {"/usr/share/conf"}},
  {Type::auxiliary, {"/usr/share/auxiliary"}}
};

vector<path> getLocationsFromEnv(const string& path_variable, bool exist_only) {

  using System::getEnv;

  string env_content = getEnv(path_variable);

  vector<string> str_list;
  boost::split(str_list, env_content, boost::is_any_of(PATH_SEP));

  vector<path> found_list(str_list.size());
  std::transform(str_list.cbegin(), str_list.cend(),
      found_list.begin(),
      [](string s){
      return path{s};
  });

  if (exist_only) {
    auto new_end = std::remove_if(found_list.begin(),
                                  found_list.end(),
                                  [](path p){
                                     return boost::filesystem::exists(p);
                                  });
    found_list.erase(new_end, found_list.end());
  }

  return found_list;
}


// Template instantiation for the most common types
template path getPathFromLocations(const path& file_name, const vector<path>& locations);
template path getPathFromLocations(const path& file_name, const vector<string>& locations);
template path getPathFromLocations(const string& file_name, const vector<path>& locations);
template path getPathFromLocations(const string& file_name, const vector<string>& locations);

template path getPathFromEnvVariable<path>(const path& file_name, const string& path_variable);
template path getPathFromEnvVariable<string>(const string& file_name, const string& path_variable);

template vector<path> multiPathAppend(const vector<path>& initial_locations, const vector<path>& suffixes);
template vector<path> multiPathAppend(const vector<path>& initial_locations, const vector<string>& suffixes);
template vector<path> multiPathAppend(const vector<string>& initial_locations, const vector<path>& suffixes);
template vector<path> multiPathAppend(const vector<string>& initial_locations, const vector<string>& suffixes);


} // Path namespace
} // Elements namespace
