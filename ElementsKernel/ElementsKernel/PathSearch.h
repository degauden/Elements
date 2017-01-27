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
#ifndef ELEMENTSKERNEL_SEARCHPATH_H_
#define ELEMENTSKERNEL_SEARCHPATH_H_

#include <string>
#include <boost/filesystem.hpp>

#include "ElementsKernel/Export.h"  // ELEMENTS_API


namespace Elements {

enum class SearchType {
  Local, Recursive
};

/**
 * @brief search
 *   Searches for a file or a directory in a directory. The search can be recursive (SearchType.Recursive)
 *   and in that case more than one results can be return
 *
 * @param searched_name
 *   Name of the searched file or directory
 * @param directory
 *   Boost path of the directory in which the search is carried out
 * @param search_type
 *   Two options:
 *    SearchType.Local search in directory only
 *    SearchType.Recursive search in sub-directories too
 * @return
 *   A vector of paths of the files found or empty string, if nothing is found
 */
ELEMENTS_API std::vector<boost::filesystem::path> pathSearch(
    const std::string& searched_name,
    boost::filesystem::path directory,
    SearchType search_type);

ELEMENTS_API std::vector<std::string> pathSearch(
    const std::string& searched_name,
    std::string directory,
    SearchType search_type);

/**
 * Iterate over the different directories included in a path-like environment variable, i.e.,
 *
 * path1:path2:path3 ...
 *
 * and call pathSearch(...) for each of them
 */
ELEMENTS_API std::vector<boost::filesystem::path> pathSearchInEnvVariable(const std::string& file_name,
                                                                          const std::string& path_like_env_variable,
                                                                          SearchType search_type=SearchType::Recursive);

//  /**
//   * @brief
//   *    Split a path using the into segments
//   * @details
//   *    Extract the last segment of a file full path variable excluding the filename. E.g.,
//   *    if path is
//   *
//   *    /Users/dubath/Elements/ElementsKernel/tests/conf/ElementsKernel/MockFileForTestingFileLocator.conf
//   *
//   *    it returns "ElementsKernel". Elements are split between "/". If the given path do not
//   *    contain any "/" it return an empty path
//   *
//   * @param fileFullPath
//   *    The path variable to be processed
//   * @return
//   *    The last segment of the path variable, or an empty path if there is no "/" in the variable.
//   */
//  static std::vector<boost::filesystem::path> tokenizer(
//      boost::filesystem::path path);
//
//  static std::vector<boost::filesystem::path> tokenizePathLikeVariable(
//      std::string path_like_env_variable);

}  // Elements namespace

#endif  // ELEMENTSKERNEL_SEARCHPATH_H_
