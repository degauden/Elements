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

#include <ostream>                      // for operator<<, basic_ostream, etc
#include <string>                       // for string, char_traits
#include <vector>                       // for vector


#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include "ElementsKernel/Exception.h"   // for Exception
#include "ElementsKernel/System.h"
#include "ElementsKernel/Logging.h"     // for the logger

using std::vector;
using std::string;

using boost::filesystem::path;
using boost::filesystem::directory_iterator;
using boost::filesystem::recursive_directory_iterator;

namespace Elements {

namespace {
  Logging logger = Logging::getLogger("PathSearch");
}

// template instanciations

template vector<string> pathSearch<string, directory_iterator>(const string& searched_name, string directory);
template vector<path> pathSearch<path, directory_iterator>(const string& searched_name, path directory);
template vector<string> pathSearch<string, recursive_directory_iterator>(const string& searched_name, string directory);
template vector<path> pathSearch<path, recursive_directory_iterator>(const string& searched_name, path directory);

template vector<string> searchOption(string searched_name, string directory, SearchType search_type);
template vector<path> searchOption(string searched_name, path directory, SearchType search_type);



template vector<path> pathSearch(const string& searched_name, path directory,
                                   SearchType search_type);
template vector<string> pathSearch(const string& searched_name, string directory,
                                     SearchType search_type);


/**
 * Iterate over the different directories included in the path-like environment variable, i.e.,
 *
 * path1:path2:path3 ...
 *
 * and call pathSearch(...) for each of them
 */
vector<path> pathSearchInEnvVariable(const std::string& file_name,
                                        const std::string& path_like_env_variable,
                                            SearchType search_type) {

  // Placeholder for the to-be-returned search result
  vector<path> search_results { };

  // get the multiple path from the environment variable
  string multiple_path {};
  if (not System::getEnv(path_like_env_variable.c_str(), multiple_path)) {
    logger.warn() << "Environment variable \"" << path_like_env_variable
                  << "\" is not defined !";
  }

  // Tokenize the path elements
  vector<string> path_elements;
  boost::split(path_elements, multiple_path, boost::is_any_of(";:"));

  // Loop over all path elements
  for (string path_element : path_elements) {
    // Check if directory exists
    if (boost::filesystem::exists(path_element) && boost::filesystem::is_directory(path_element)) {
      // loop recursively inside directory
      auto single_path_results = pathSearch(file_name,
                                            path { path_element },
                                            search_type);
      for (path aPath : single_path_results) {
        search_results.push_back(aPath);
      }
    }
  }
  return search_results;
}

} // Elements namespace
