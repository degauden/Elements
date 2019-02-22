/**
 * @file ElementsKernel/PathSearch.h
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

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_PATHSEARCH_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_PATHSEARCH_H_

#include <string>
#include <vector>
#include <boost/filesystem.hpp>

#include "ElementsKernel/Export.h"     // ELEMENTS_API

namespace Elements {

enum class SearchType {
  Local, Recursive
};

/**
 * @brief
 *   Searches for a file or a directory in a directory. The search can be recursive (SearchType.Recursive)
 *   and in that case more than one results can be return
 * @param searched_name
 *   Name of the searched file or directory
 * @tparam T
 *   input type string or Boost path of the directory in which the search is carried out
 * @param directory
 *   The directory where the search is performed.
 * @param search_type
 *   Two options:
 *    SearchType.Local search in directory only
 *    SearchType.Recursive search in sub-directories too
 * @return
 *   A vector of paths of the files found or empty string, if nothing is found
 */
template <typename T>
ELEMENTS_API std::vector<T> pathSearch(
    const std::string& searched_name,
    T directory,
    SearchType search_type);

// template instantiations of the most common types
extern template
ELEMENTS_API std::vector<boost::filesystem::path> pathSearch(const std::string& searched_name,
                                                             boost::filesystem::path directory, SearchType search_type);
extern template
ELEMENTS_API std::vector<std::string> pathSearch(const std::string& searched_name, std::string directory, SearchType search_type);


/**
 * @brief
 *   Searches for a file or a directory in a path pointed by an environment variable.
 *   It can contains collection of colon separated locations.
 *   The search can be recursive (SearchType.Recursive)
 *   and in that case more than one results can be return
 * @param file_name
 *   Name of the searched file or directory
 * @param path_like_env_variable
 *   The environment variable name that contains the list of directories
 * @param search_type
 *   Two options:
 *    SearchType.Local search in directory only
 *    SearchType.Recursive search in sub-directories too
 * @return
 *   A vector of paths of the files found or empty string, if nothing is found
 */
ELEMENTS_API
std::vector<boost::filesystem::path> pathSearchInEnvVariable(
    const std::string& file_name,
    const std::string& path_like_env_variable,
    SearchType search_type = SearchType::Recursive);

}  // namespace Elements

#include "ElementsKernel/_impl/PathSearch.icpp"


#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_PATHSEARCH_H_

/**@}*/
