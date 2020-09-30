/**
 * @file ElementsKernel/Path.h
 *
 * @brief provide functions to retrieve resources
 *   pointed by environment variables
 * @date May 13, 2016
 * @author Hubert Degaudenzi
 *
 * @copyright 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation; either version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this
 * library; if not, write to the
 *
 * Free Software Foundation, Inc.,
 * 51 Franklin Street,
 * Fifth Floor,
 * Boston, MA 02110-1301 USA
 *
 *
 */

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_PATH_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_PATH_H_

#include <boost/filesystem.hpp>  // for boost::filesystem
#include <map>                   // for map
#include <string>                // for string
#include <utility>               // for forward
#include <vector>                // for vector

#include "ElementsKernel/Export.h"  // ELEMENTS_API

namespace Elements {
namespace Path {

enum class Type { executable, library, python, configuration, auxiliary };

/**
 * @typedef local alias for the path
 * @ingroup ElementsKernel
 */
using Item = boost::filesystem::path;

/**
 * @brief Separator of path entries. Usually ":" on Unix.
 * @ingroup ElementsKernel
 */
ELEMENTS_API extern const std::string PATH_SEP;

/**
 * @brief map containing the name of the path variable for each type
 * @ingroup ElementsKernel
 */
ELEMENTS_API extern const std::map<Type, const std::string> VARIABLE;

/**
 * @brief map containing the default project installation
 * suffixes for each variable
 * @ingroup ElementsKernel
 */
ELEMENTS_API extern const std::map<Type, const std::vector<std::string>> SUFFIXES;

/**
 * @brief map containing the default external locations for each variable
 * @ingroup ElementsKernel
 */
ELEMENTS_API extern const std::map<Type, const std::vector<std::string>> DEFAULT_LOCATIONS;

/**
 * @brief map containing the sub-level property of the path components
 * @ingroup ElementsKernel
 */
ELEMENTS_API extern const std::map<Type, const bool> HAS_SUBLEVELS;

/**
 * @brief function to get the locations from an environment variable
 * @ingroup ElementsKernel
 * @details
 *    This function return the raw locations pointed by the environment
 *    variable. It doesn't add the internal locations which are not in
 *    the variable (like /usr/lib for the LD_LIBRARY_PATH environment variable)
 * @param path_variable
 *    name of the environment variable
 * @param exist_only
 *    if true returns only existing locations. by default it is set
 *    to false.
 * @return
 *    return a list of boost filesystem paths
 */
ELEMENTS_API std::vector<Item> getLocationsFromEnv(const std::string& path_variable, bool exist_only = false);

/**
 * @brief function to get the locations for the specific type
 * @ingroup ElementsKernel
 * @details
 *    This function return the raw locations for the given type.
 *    It doesn't add the internal locations which are not in
 *    the variable (like /usr/lib for the library type)
 * @param path_type
 *    type of the path
 * @param exist_only
 *    if true returns only existing locations. by default it is set
 *    to false.
 * @return
 *    return a list of boost filesystem paths
 */
ELEMENTS_API std::vector<Item> getLocations(const Type& path_type, bool exist_only = false);

/**
 * @brief retrieve path from a file name and a set of location to look into
 * @ingroup ElementsKernel
 * @param file_name
 *   file name to look for. Can be of the form "Some.txt" or "Place/Some.txt"
 * @param locations
 *   vector of locations to look into
 * @tparam T
 *   type of the file name. Can be anything that can be converted to a boost
 *   filesystem path. In principle either std::string or path.
 * @tparam U
 *   type of the location. Can be anything that can be converted to a boost
 *   filesystem path. In principle either std::string or path.
 * @return
 *   first match of the file stem
 */
template <typename T, typename U>
ELEMENTS_API Item getPathFromLocations(const T& file_name, const std::vector<U>& locations);
// Template instantiation for the most common types
extern template ELEMENTS_API Item getPathFromLocations(const Item& file_name, const std::vector<Item>& locations);
extern template ELEMENTS_API Item getPathFromLocations(const Item& file_name, const std::vector<std::string>& locations);
extern template ELEMENTS_API Item getPathFromLocations(const std::string& file_name, const std::vector<Item>& locations);
extern template ELEMENTS_API Item getPathFromLocations(const std::string& file_name, const std::vector<std::string>& locations);

/**
 * @brief retrieve all the paths from a file name and a set of location to look into
 * @ingroup ElementsKernel
 * @param file_name
 *   file name to look for. Can be of the form "Some.txt" or "Place/Some.txt"
 * @param locations
 *   vector of locations to look into
 * @tparam T
 *   type of the file name. Can be anything that can be converted to a boost
 *   filesystem path. In principle either std::string or path.
 * @tparam U
 *   type of the location. Can be anything that can be converted to a boost
 *   filesystem path. In principle either std::string or path.
 * @return
 *   all the found paths
 */
template <typename T, typename U>
ELEMENTS_API std::vector<Item> getAllPathFromLocations(const T& file_name, const std::vector<U>& locations);
// Template instantiation for the most common types
extern template ELEMENTS_API std::vector<Item> getAllPathFromLocations(const Item& file_name, const std::vector<Item>& locations);
extern template ELEMENTS_API std::vector<Item> getAllPathFromLocations(const Item&                     file_name,
                                                                       const std::vector<std::string>& locations);
extern template ELEMENTS_API std::vector<Item> getAllPathFromLocations(const std::string&       file_name,
                                                                       const std::vector<Item>& locations);
extern template ELEMENTS_API std::vector<Item> getAllPathFromLocations(const std::string&              file_name,
                                                                       const std::vector<std::string>& locations);

/**
 * @brief retrieve path from a file name and an environment variable to look into
 * @ingroup ElementsKernel
 * @param file_name
 *   file name to look for. Can be of the form "Some.txt" or "Place/Some.txt"
 * @param path_variable
 *   name of the environment variable
 * @tparam T
 *   type of the file name. Can be anything that can be converted to a boost
 *   filesystem path. In principle either std::string or path.
 * @return
 *   first match of the file stem
 */
template <typename T>
ELEMENTS_API Item getPathFromEnvVariable(const T& file_name, const std::string& path_variable);
// Template instantiation for the most common types
extern template ELEMENTS_API Item getPathFromEnvVariable<Item>(const Item& file_name, const std::string& path_variable);
extern template ELEMENTS_API Item getPathFromEnvVariable<std::string>(const std::string& file_name,
                                                                      const std::string& path_variable);

/**
 * @brief collate a vector of path into a string using PATH_SEP
 * @ingroup ElementsKernel
 * @param path_list
 *   list of path to be joined.
 * @return collated string
 */
template <typename T>
ELEMENTS_API std::string joinPath(const std::vector<T>& path_list);
// Template instantiation for the most common types
extern template ELEMENTS_API std::string joinPath(const std::vector<Item>& path_list);

extern template ELEMENTS_API std::string joinPath(const std::vector<std::string>& path_list);

/**
 * @brief alias for the joinPath function
 * @ingroup ElementsKernel
 * @param args
 *   Forward arguments
 * @return same as joinPath
 */
template <typename... Args>
ELEMENTS_API auto join(Args&&... args) -> decltype(joinPath(std::forward<Args>(args)...));

/**
 * @brief split a string into a vector of path using PATH_SEP
 * @ingroup ElementsKernel
 * @param path_string
 *   string containing a list of path separated by PATH_SEP
 * @return vector of path
 */
ELEMENTS_API std::vector<Item> splitPath(const std::string& path_string);

/**
 * @brief alias for the splitPath function
 * @ingroup ElementsKernel
 * @param args
 *   Forward arguments
 * @return same as splitPath
 */
template <typename... Args>
ELEMENTS_API auto split(Args&&... args) -> decltype(splitPath(std::forward<Args>(args)...));

/**
 * @brief path join each suffix to each initial locations
 * @ingroup ElementsKernel
 * @param initial_locations
 *   list of initial locations.
 * @param suffixes
 *   list of suffixes
 * @return full list of each joined item
 */
template <typename T, typename U>
ELEMENTS_API std::vector<Item> multiPathAppend(const std::vector<T>& initial_locations, const std::vector<U>& suffixes);
// Template instantiation for the most common types
extern template ELEMENTS_API std::vector<Item> multiPathAppend(const std::vector<Item>& initial_locations,
                                                               const std::vector<Item>& suffixes);
extern template ELEMENTS_API std::vector<Item> multiPathAppend(const std::vector<Item>&        initial_locations,
                                                               const std::vector<std::string>& suffixes);
extern template ELEMENTS_API std::vector<Item> multiPathAppend(const std::vector<std::string>& initial_locations,
                                                               const std::vector<Item>&        suffixes);
extern template ELEMENTS_API std::vector<Item> multiPathAppend(const std::vector<std::string>& initial_locations,
                                                               const std::vector<std::string>& suffixes);
/**
 * @brief remove duplicated paths keeping the order
 * @ingroup ElementsKernel
 * @param path_list
 *   list of path to uniquify.
 * @return deduplicated list of path
 */
template <typename T>
ELEMENTS_API std::vector<Item> removeDuplicates(const std::vector<T>& path_list);
// Template instantiation for the most common types
extern template ELEMENTS_API std::vector<Item> removeDuplicates(const std::vector<Item>& path_list);

extern template ELEMENTS_API std::vector<Item> removeDuplicates(const std::vector<std::string>& path_list);

}  // namespace Path
}  // namespace Elements

#define ELEMENTSKERNEL_ELEMENTSKERNEL_PATH_IMPL_
#include "ElementsKernel/_impl/Path.icpp"
#undef ELEMENTSKERNEL_ELEMENTSKERNEL_PATH_IMPL_

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_PATH_H_

/**@}*/
