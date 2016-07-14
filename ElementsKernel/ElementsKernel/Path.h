/**
 * @file ElementsKernel/Path.h
 *
 * @brief provide functions to retrieve ressources
 *   pointed by environment variables
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
 * @brief map containing the name of the path variable for each type
 */
ELEMENTS_API extern std::map<Type, std::string> VARIABLE;

/**
 * @brief map containing the default project installation
 * suffixes for each variable
 */
ELEMENTS_API extern std::map<Type, std::vector<std::string>> SUFFFIXES;

/**
 * @brief map containing the default external locations for each variable
 */
ELEMENTS_API extern std::map<Type, std::vector<std::string>> DEFAULT_LOCATIONS;

/**
 * @brief function to get the locations from an environment variable
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
ELEMENTS_API std::vector<boost::filesystem::path> getLocationsFromEnv(const std::string& path_variable, bool exist_only=false);

/**
 * @brief retrieve path from a file name and a set of location to look into
 * @param file_name
 *   file name to look for. Can be of the form "Some.txt" or "Place/Some.txt"
 * @param locations
 *   vector of locations to look into
 * @tparam T
 *   type of the file name. Can be anything that can be converted to a boost
 *   filesystem path. In principle either std::string or fs::path.
 * @tparam U
 *   type of the location. Can be anything that can be converted to a boost
 *   filesystem path. In principle either std::string or fs::path.
 * @return
 *   first match of the file stem
 */
template <typename T, typename U>
ELEMENTS_API boost::filesystem::path getPathFromLocations(const T& file_name, const std::vector<U>& locations);

/**
 * @brief retrieve path from a file name and an environment variable to look into
 * @param file_name
 *   file name to look for. Can be of the form "Some.txt" or "Place/Some.txt"
 * @param path_variable
 *   name of the environment variable
 * @tparam T
 *   type of the file name. Can be anything that can be converted to a boost
 *   filesystem path. In principle either std::string or fs::path.
 * @return
 *   first match of the file stem
 */
template <typename T>
ELEMENTS_API boost::filesystem::path getPathFromEnvVariable(const T& file_name, const std::string& path_variable);

}  // Path namespace
}  // Elements namespace

#include "ElementsKernel/_impl/Path.icpp"

#endif // ELEMENTSKERNEL_ELEMENTSKERNEL_PATH_H_
