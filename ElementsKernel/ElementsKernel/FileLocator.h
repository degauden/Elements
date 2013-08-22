/**
 * @file FileLocator.h
 *
 * Created on: Aug 5, 2013
 *     Author: Pavel Binko
 */

#ifndef FILELOCATOR_H_
#define FILELOCATOR_H_

#include <string>
#include "boost/filesystem.hpp"
#include <boost/regex.hpp>

/**
 * @brief searchFileInEnvVariable
 *   Searches for a file in all directories contained
 *   in an environment variable
 * @param pathEnvVariable
 *   Name of an environment variable, which should have the format
 *   VAR=path1:path1:...:pathN
 * @param filePath
 *   Boost path of a file, from which the filename will be extracted
 *   (extension will be replace by below extension)
 * @param extension
 *   Search file extension.
 * @return
 *   Full path to the file found or empty string, if not found
 */
boost::filesystem::path searchFileInPath(std::string pathEnvVariable,
    boost::filesystem::path file, std::string extension);

/**
 * @brief
 *    Extract the last segment of a file full path variable excluding the filename.
 * @details
 *    Extract the last segment of a file full path variable excluding the filename. E.g.,
 *    if path is
 *
 *    /Users/dubath/Elements/ElementsKernel/tests/conf/ElementsKernel/MockFileForTestingFileLocator.conf
 *
 *    it returns "ElementsKernel". Elements are split between "/". If the given path do not
 *    contain any "/" it return an empty path
 *
 * @param pathElement
 *    The path variable to be processed
 * @return
 *    The last segment of the path variable, or an empty path if there is no "/" in the variable.
 */
boost::filesystem::path getPathLastSegment(boost::filesystem::path fileFullPath);

/**
 * @brief
 *    Extract the full path of all files with a given name found in the directory and its sub-directories
 * @details
 *    If more than one file with the given name are detected, they full path will differ.
 *
 * @param directoryName
 *    The directory that will be recursively searched
 * @param searchedFileName
 *    The name (with extension, biut without path) of the searched file
 * @return
 *    A vector of full path of the one (or many) detected file(s)
 */
std::vector<boost::filesystem::path> searchFileRecursivelyInDir(std::string directoryName,
    std::string searchedFileName);

/**
 * @brief
 *    Check if the last element of the full paths of the files match that given by the prefix string
 * @details
 *    When provided with a vector of full paths of a number of files with the same name+extension, it return another
 *    vector with all the files, which full path last element match the prefix.
 *    E.g., if prefix is "ElementsKernel",
 *
 *    /Users/dubath/Elements/ElementsKernel/tests/conf/ElementsKernel/MockFileForTestingFileLocator.conf
 *    /Users/dubath/Elements/ElementsKernel/tests/conf/MockFileForTestingFileLocator.conf
 *
 *    the first above entry will be in the returned vector while the second will not.
 *
 * @param fileFullPaths
 *    A vector of full paths of files with identical name+extension
 * @param prefix
 *    The prefix to be checked
 * @return
 *    A vector with all file full paths whose last element match the prefix
 */
std::vector<boost::filesystem::path> checkPathPrefix(std::vector<boost::filesystem::path> fileFullPaths,
    std::string prefix);

/**
 * @brief
 *    Select one file from a list of file with identical name but different paths
 * @details
 *    This is used for selecting a configuration file from a set of many that could be
 *    found in a given location. Current the selection is not yet implemented. It return
 *    one element if the input vector contains one element and it throws an exception
 *    if the input vector includes more than one elements.
 * @param checkedFiles
 *    Vector of files
 * @return
 *    The selected file
 */
boost::filesystem::path selectFileInVector(std::vector<boost::filesystem::path> checkedFiles);




boost::filesystem::path searchConfFileRecursivelyInDir(std::string pathElement,
    boost::regex searchedFileName, boost::filesystem::path prefixPath) nothrows;

/**
 * @brief searchConfFileInPath
 *   Searches for a configuration file recursively in all directories/sub-directories given
 *   in an environment variable
* @param fileName
 *   File name of the configuration file (only the root name is used as
 *   extension is replaced by configurationExtension in any case)
 * @param pathPrefix
 *   The path of the parent directory containing the sought configuration file. By default this is not used.
 * @param extension
 *   Configuration file extension (defaulted to "conf")
 * @param pathConfEnvVariable
 *   Name of the configuration environment variable defaulted to "ELEMENTS_CONF_PATH".
 *   It should have the format: ELEMENTS_CONF_PATH=path1:path2:...:pathN
 * @return
 *   Full path to the configuration file found or empty string, if not found,
 * @throws ElementsException
 *   If the configuration file is found more than once in a given directory/sun-directories.
 */
boost::filesystem::path searchConfFileInPathVariable(
    std::string fileName, std::string pathPrefix = "",
    std::string configurationExtension = "conf",
    std::string pathConfEnvVariable = "ELEMENTS_CONF_PATH");

//  /**
//   * @brief tokenizeEnvVariable
//   *   Tokenizes an environment variable
//   * @param envVariable
//   *   Name of an envirnment variable, which should have the format
//   *   VAR=path1:path1:...:pathN
//   * @return
//   *   Vector of tokens (token is an individual pathX)
//   */
//  //std::vector<std::string> tokenizeEnvVariable(const std::string & envVariable);
//private:
//
//  /**
//   * @brief searchForFileInDir
//   *   Finds a file in a directory
//   * @param dir_path
//   *   Full path to a directory, where the search will take place
//   * @param file_name
//   *   File name to be searched for
//   * @return
//   *   Full path to the file found (empty string, if not found)
//   */
////  static bool searchForFileInDir(const boost::filesystem::path & dir_path,
////      const std::string & file_name, boost::filesystem::path & path_found);
//};

#endif /* FILELOCATOR_H_ */
