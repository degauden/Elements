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

#include <algorithm>  // for transform, remove_if
#include <map>        // for map
#include <string>     // for string
#include <vector>     // for vector

#include <boost/algorithm/string.hpp>  // for boost::split
#include <boost/filesystem.hpp>        // for boost::filesystem

#include "ElementsKernel/Environment.h"  // for the Environment class
#include "ElementsKernel/System.h"       // for getEnv, SHLIB_VAR_NAME

using std::map;
using std::string;
using std::vector;

namespace Elements {
inline namespace Kernel {
namespace Path {

const string PATH_SEP{":"};

const map<Type, const string> VARIABLE{{Type::executable, "PATH"},
                                       {Type::library, System::SHLIB_VAR_NAME},
                                       {Type::python, "PYTHONPATH"},
                                       {Type::configuration, "ELEMENTS_CONF_PATH"},
                                       {Type::auxiliary, "ELEMENTS_AUX_PATH"}};

const map<Type, const vector<string>> SUFFIXES{{Type::executable, {"scripts", "bin"}},
                                               {Type::library, {"lib"}},
                                               {Type::python, {"python"}},
                                               {Type::configuration, {"conf", "share/conf"}},
                                               {Type::auxiliary, {"auxdir", "aux", "share/auxdir", "share/aux"}}};

const map<Type, const vector<string>> DEFAULT_LOCATIONS{{Type::executable, {}},
                                                        {Type::library, {"/usr/lib64", "/usr/lib"}},
                                                        {Type::python, {}},
                                                        {Type::configuration, {"/usr/share/conf"}},
                                                        {Type::auxiliary, {"/usr/share/auxdir", "/usr/share/aux"}}};

const std::map<Type, const bool> HAS_SUBLEVELS{{Type::executable, false},
                                               {Type::library, false},
                                               {Type::python, true},
                                               {Type::configuration, true},
                                               {Type::auxiliary, true}};

vector<Item> getLocationsFromEnv(const string& path_variable, bool exist_only) {

  Environment current_env;

  string env_content = current_env[path_variable];

  vector<Item> found_list = split(env_content);

  if (exist_only) {
    auto new_end = std::remove_if(found_list.begin(), found_list.end(), [](const Item& p) {
      return (not boost::filesystem::exists(p));
    });
    found_list.erase(new_end, found_list.end());
  }

  return found_list;
}

vector<Item> getLocations(const Type& path_type, bool exist_only) {
  return getLocationsFromEnv(VARIABLE.at(path_type), exist_only);
}

vector<Item> splitPath(const string& path_string) {

  vector<string> str_list;
  boost::split(str_list, path_string, boost::is_any_of(PATH_SEP));

  vector<Item> found_list(str_list.size());
  std::transform(str_list.cbegin(), str_list.cend(), found_list.begin(), [](const string& s) {
    return Item{s};
  });

  return found_list;
}

// Template instantiation for the most common types
template Item getPathFromLocations(const Item& file_name, const vector<Item>& locations);
template Item getPathFromLocations(const Item& file_name, const vector<string>& locations);
template Item getPathFromLocations(const string& file_name, const vector<Item>& locations);
template Item getPathFromLocations(const string& file_name, const vector<string>& locations);

template vector<Item> getAllPathFromLocations(const Item& file_name, const vector<Item>& locations);
template vector<Item> getAllPathFromLocations(const Item& file_name, const vector<string>& locations);
template vector<Item> getAllPathFromLocations(const string& file_name, const vector<Item>& locations);
template vector<Item> getAllPathFromLocations(const string& file_name, const vector<string>& locations);

template Item getPathFromEnvVariable<Item>(const Item& file_name, const string& path_variable);
template Item getPathFromEnvVariable<string>(const string& file_name, const string& path_variable);

template string joinPath(const vector<Item>& path_list);
template string joinPath(const vector<string>& path_list);

template vector<Item> multiPathAppend(const vector<Item>& initial_locations, const vector<Item>& suffixes);
template vector<Item> multiPathAppend(const vector<Item>& initial_locations, const vector<string>& suffixes);
template vector<Item> multiPathAppend(const vector<string>& initial_locations, const vector<Item>& suffixes);
template vector<Item> multiPathAppend(const vector<string>& initial_locations, const vector<string>& suffixes);

template vector<Item> removeDuplicates(const vector<Item>& path_list);
template vector<Item> removeDuplicates(const vector<string>& path_list);

}  // namespace Path
}  // namespace Kernel
}  // namespace Elements
