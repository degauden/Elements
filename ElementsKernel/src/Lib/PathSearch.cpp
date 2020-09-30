/**
 * @file PathSearch.cpp
 *
 * @date Dec 4, 2013
 * @author Pierre Dubath
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

#include "ElementsKernel/PathSearch.h"  // for SearchType, etc

#include <ostream>  // for operator<<, basic_ostream, etc
#include <string>   // for string, char_traits
#include <vector>   // for vector

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/operations.hpp>

#include "ElementsKernel/Exception.h"  // for Exception
#include "ElementsKernel/Logging.h"    // for the logger
#include "ElementsKernel/Path.h"       // for Path::Item
#include "ElementsKernel/System.h"

using std::string;
using std::vector;

using boost::filesystem::directory_iterator;
using boost::filesystem::recursive_directory_iterator;

namespace Elements {

namespace {
auto log = Logging::getLogger("PathSearch");
}

// template instantiations

template vector<string>     pathSearch<string, directory_iterator>(const string& searched_name, string directory);
template vector<Path::Item> pathSearch<Path::Item, directory_iterator>(const string& searched_name, Path::Item directory);
template vector<string>     pathSearch<string, recursive_directory_iterator>(const string& searched_name, string directory);
template vector<Path::Item> pathSearch<Path::Item, recursive_directory_iterator>(const string& searched_name, Path::Item directory);

template vector<Path::Item> pathSearch(const string& searched_name, Path::Item directory, SearchType search_type);
template vector<string>     pathSearch(const string& searched_name, string directory, SearchType search_type);

/**
 * Iterate over the different directories included in the path-like environment variable, i.e.,
 *
 * path1:path2:path3 ...
 *
 * and call pathSearch(...) for each of them
 */
vector<Path::Item> pathSearchInEnvVariable(const string& file_name, const string& path_like_env_variable, SearchType search_type) {
  // Placeholder for the to-be-returned search result
  vector<Path::Item> search_results{};

  // get the multiple path from the environment variable
  string multiple_path{};
  if (not System::getEnv(path_like_env_variable.c_str(), multiple_path)) {
    log.warn() << "Environment variable \"" << path_like_env_variable << "\" is not defined !";
  }

  // Tokenize the path elements
  vector<string> path_elements;
  boost::split(path_elements, multiple_path, boost::is_any_of(";:"));

  // Loop over all path elements
  for (string path_element : path_elements) {
    // Check if directory exists
    if (boost::filesystem::exists(path_element) && boost::filesystem::is_directory(path_element)) {
      // loop recursively inside directory
      auto single_path_results = pathSearch(file_name, Path::Item{path_element}, search_type);
      search_results.insert(search_results.end(), single_path_results.cbegin(), single_path_results.cend());
    }
  }
  return search_results;
}

}  // namespace Elements
