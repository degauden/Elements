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

#include <string>                   // for string
#include <vector>                   // for vector
#include <map>                      // for map
#include <boost/filesystem.hpp>     // for boost::filesystem

#include "ElementsKernel/Export.h"  // ELEMENTS_API

namespace Elements {
namespace Path {

enum class Type {
  executable,
  library,
  python,
  configuration,
  auxiliary
};

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
ELEMENTS_API std::vector<boost::filesystem::path> getLocationsFromEnv(const std::string& path_variable,
                                                                      bool exist_only = false);

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
ELEMENTS_API boost::filesystem::path getPathFromLocations(const T& file_name, const std::vector<U>& locations);
// Template instantiation for the most common types
extern template
ELEMENTS_API boost::filesystem::path getPathFromLocations(const boost::filesystem::path& file_name,
                                                          const std::vector<boost::filesystem::path>& locations);
extern template
ELEMENTS_API boost::filesystem::path getPathFromLocations(const boost::filesystem::path& file_name,
                                                          const std::vector<std::string>& locations);
extern template
ELEMENTS_API boost::filesystem::path getPathFromLocations(const std::string& file_name,
                                                          const std::vector<boost::filesystem::path>& locations);
extern template
ELEMENTS_API boost::filesystem::path getPathFromLocations(const std::string& file_name,
                                                          const std::vector<std::string>& locations);


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
ELEMENTS_API std::vector<boost::filesystem::path> getAllPathFromLocations(const T& file_name,
                                                                          const std::vector<U>& locations);
// Template instantiation for the most common types
extern template
ELEMENTS_API std::vector<boost::filesystem::path> getAllPathFromLocations(const boost::filesystem::path& file_name,
                                                                          const std::vector<boost::filesystem::path>& locations);
extern template
ELEMENTS_API std::vector<boost::filesystem::path> getAllPathFromLocations(const boost::filesystem::path& file_name,
                                                                          const std::vector<std::string>& locations);
extern template
ELEMENTS_API std::vector<boost::filesystem::path> getAllPathFromLocations(const std::string& file_name,
                                                                          const std::vector<boost::filesystem::path>& locations);
extern template
ELEMENTS_API std::vector<boost::filesystem::path> getAllPathFromLocations(const std::string& file_name,
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
ELEMENTS_API boost::filesystem::path getPathFromEnvVariable(const T& file_name, const std::string& path_variable);
// Template instantiation for the most common types
extern template
ELEMENTS_API boost::filesystem::path getPathFromEnvVariable<boost::filesystem::path>(const boost::filesystem::path& file_name,
                                                                                     const std::string& path_variable);
extern template
ELEMENTS_API boost::filesystem::path getPathFromEnvVariable<std::string>(const std::string& file_name,
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
template <typename T>
ELEMENTS_API std::string joinPath(const std::vector<boost::filesystem::path>& path_list);

template <typename T>
ELEMENTS_API std::string joinPath(const std::vector<std::string>& path_list);


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
ELEMENTS_API std::vector<boost::filesystem::path> multiPathAppend(const std::vector<T>& initial_locations,
                                                                    const std::vector<U>& suffixes);
// Template instantiation for the most common types
extern template
ELEMENTS_API std::vector<boost::filesystem::path> multiPathAppend(const std::vector<boost::filesystem::path>& initial_locations,
                                                             const std::vector<boost::filesystem::path>& suffixes);
extern template
ELEMENTS_API std::vector<boost::filesystem::path> multiPathAppend(const std::vector<boost::filesystem::path>& initial_locations,
                                                             const std::vector<std::string>& suffixes);
extern template
ELEMENTS_API std::vector<boost::filesystem::path> multiPathAppend(const std::vector<std::string>& initial_locations,
                                                             const std::vector<boost::filesystem::path>& suffixes);
extern template
ELEMENTS_API std::vector<boost::filesystem::path> multiPathAppend(const std::vector<std::string>& initial_locations,
                                                             const std::vector<std::string>& suffixes);

}  // namespace Path
}  // namespace Elements

#include "ElementsKernel/_impl/Path.icpp"

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_PATH_H_

/**@}*/
