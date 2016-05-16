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

namespace fs = boost::filesystem;

using std::vector;
using std::string;

namespace Elements {

static Logging logger = Logging::getLogger("PathSearch");

//-----------------------------------------------------------------------------
// Function search
template<typename T, typename ITER>
vector<T> pathSearch(std::string searched_name, T directory) {

  // create the resulting vector
  vector<T> searchResults { };
  // make sure directory is ps::path, changing from string to path if T is string.
  fs::path l_directory { directory };
  // the default constructor of ITER return a pointer to one-past last element
  ITER end_iter;
  if (fs::is_directory(l_directory)) {
    // ITER constructor return a pointer to the first element of l_directory
    for (ITER dir_iter(l_directory); dir_iter != end_iter; ++dir_iter) {
      if (dir_iter->path().filename() == searched_name) {
        // File found: make sure the result is T: string to string or string to fs::path
        T l_result { dir_iter->path().string() };
        searchResults.push_back(l_result);
      }
    }
  }
  return searchResults;
}

template<typename T>
vector<T> searchOption(string searched_name, T directory,
    SearchType search_type) {

  // create a local tmp vector result to avoid multiple return statements
  vector<T> searchResults { };
  switch (search_type) {
  case SearchType::Local:
    searchResults = pathSearch<T, fs::directory_iterator>(searched_name,
        directory);
    break;
  case SearchType::Recursive:
    searchResults = pathSearch<T, fs::recursive_directory_iterator>(
        searched_name, directory);
    break;
  }
  return searchResults;
}

vector<fs::path> pathSearch(string searched_name, fs::path directory,
    SearchType search_type) {
  return searchOption<fs::path>(searched_name, directory, search_type);
}

vector<string> pathSearch(string searched_name, string directory,
    SearchType search_type) {
  return searchOption<string>(searched_name, directory, search_type);
}

/**
 * Iterate over the different directories included in the path-like environment variable, i.e.,
 *
 * path1:path2:path3 ...
 *
 * and call pathSearch(...) for each of them
 */
vector<fs::path> pathSearchInEnvVariable(std::string file_name,
                                            std::string path_like_env_variable,
                                            SearchType search_type) {

  // Placeholder for the to-be-returned search result
  vector<fs::path> search_results { };

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
    if (fs::exists(path_element) && fs::is_directory(path_element)) {
      // loop recursively inside directory
      auto single_path_results = pathSearch(file_name,
                                            fs::path { path_element },
                                            search_type);
      for (fs::path aPath : single_path_results) {
        search_results.push_back(aPath);
      }
    }
  }
  return search_results;
}

} // Elements namespace
