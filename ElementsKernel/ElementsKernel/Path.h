/**
 * @file Path.h
 * @date Dec 4, 2013
 * @author Pierre Dubath
 */
#ifndef PATH_H_
#define PATH_H_

#include <string>
#include <vector>
#include "boost/filesystem.hpp"
#include <boost/regex.hpp>
#include "ElementsKernel/Export.h" // ELEMENTS_API

typedef enum {
    Local, Recursive
  } SearchType;

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
   ELEMENTS_API std::vector<boost::filesystem::path> pathSearch(std::string searched_name,
      boost::filesystem::path directory, SearchType search_type);

   ELEMENTS_API std::vector<std::string> pathSearch(std::string searched_name,
       std::string directory, SearchType search_type);

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


#endif /* PATH_H_ */
