/**
 * @file ElementsKernel/_impl/PathSearch.tpp
 * @brief implementation of the templates declared in ElementsKernel/Path.h
 * @date May 17, 2016
 * @author Hubert Degaudenzi
 *
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
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_PATHSEARCH_IMPL_
#error "This file should not be included directly! Use ElementsKernel/Path.h instead"
#else

#include <string>
#include <vector>

#include <boost/filesystem/operations.hpp>

#include "ElementsKernel/Path.h"

namespace Elements {
inline namespace Kernel {
//-----------------------------------------------------------------------------
// Function search
template <typename T, typename ITER>
std::vector<T> pathSearch(const std::string& searched_name, T directory) {

  // create the resulting vector
  std::vector<T> searchResults{};
  // make sure directory is ps::path, changing from string to path if T is string.
  Path::Item l_directory{directory};
  // the default constructor of ITER return a pointer to one-past last element
  ITER end_iter;
  if (boost::filesystem::is_directory(l_directory)) {
    // ITER constructor return a pointer to the first element of l_directory
    for (ITER dir_iter(l_directory); dir_iter != end_iter; ++dir_iter) {
      if (dir_iter->path().filename() == searched_name) {
        // File found: make sure the result is T: string to string or string to
        // boost::filesystem::path
        T l_result{dir_iter->path().string()};
        searchResults.emplace_back(l_result);
      }
    }
  }
  return searchResults;
}

template <typename T>
std::vector<T> searchOption(std::string searched_name, T directory, SearchType search_type) {

  // create a local tmp vector result to avoid multiple return statements
  std::vector<T> searchResults{};
  switch (search_type) {
  case SearchType::Local:
    searchResults = pathSearch<T, boost::filesystem::directory_iterator>(searched_name, directory);
    break;
  case SearchType::Recursive:
    searchResults = pathSearch<T, boost::filesystem::recursive_directory_iterator>(searched_name, directory);
    break;
  }
  return searchResults;
}

template <typename T>
std::vector<T> pathSearch(const std::string& searched_name, T directory, SearchType search_type) {
  return searchOption<T>(searched_name, directory, search_type);
}

}  // namespace Kernel
}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_PATHSEARCH_IMPL_
