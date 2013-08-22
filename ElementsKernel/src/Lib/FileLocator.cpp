/**
 * @file FileLocator.cpp
 *
 * Created on: Aug 5, 2013
 *     Author: Pavel Binko
 */

#include <iostream>
#include <boost/algorithm/string.hpp>

#include "ElementsKernel/FileLocator.h"
#include "ElementsKernel/ElementsException.h"

namespace fs = boost::filesystem;
using namespace std;

//-----------------------------------------------------------------------------
// Function searchInEnvVariable
fs::path searchFileInPath(std::string pathEnvVariable, fs::path file,
    std::string extension) {

  // Placeholder for the to-be-returned search result
  fs::path searchResult { };

  // get the path from the environment variable
  std::string composedPath = getenv(pathEnvVariable.c_str());

  // Define the file name to be searched for
  fs::path fileName = file.filename().replace_extension(extension);

  // Tokenize the path elements
  vector<string> pathElements;
  boost::split(pathElements, composedPath, boost::is_any_of(";:"));

  // Loop over all path elements
  for (string pathElement : pathElements) {
    fs::directory_iterator end_iter;
    if (fs::exists(pathElement) && fs::is_directory(pathElement)) {
      for (fs::directory_iterator dir_iter(pathElement); dir_iter != end_iter;
          ++dir_iter) {
        if (dir_iter->path().filename() == fileName) {
          // File found
          searchResult = dir_iter->path();
        }
      }
    }
  }

  return searchResult;

} // Eof FileLocator::searchInEnvVariable

/*
 * This method is required in the following searchConfFileInPath
 */
fs::path getPathLastSegment(fs::path fileFullPath) {
  // place holder for result
  fs::path lastPathSegment { };
  // vector to store path elements
  vector<string> prefixElements { };
  // split the path
  boost::split(prefixElements, fileFullPath.parent_path().string(),
      boost::is_any_of("/"));
  if (!prefixElements.empty()) {
    lastPathSegment = prefixElements.back();
  }
  return lastPathSegment;
}

/*
 * In this search, we first search recursively the file into the different directories provided
 * by the path. When a file which match the provided name is found, the prefix (if provided) is checked.
 * If it does not match, the search for another file match continues.
 */
vector<fs::path> searchFileRecursivelyInDir(string directoryName,
    string searchedFileName) {

  // placeholder for the search result
  vector<fs::path> searchResults { };
  // from string to regex types
  boost::regex searchedFile { searchedFileName };

  // Iterate over all files and sub-directories
  for (fs::recursive_directory_iterator iter(directoryName), end; iter != end;
      ++iter) {

    // Get the name of a directory element (file or directory itself)
    std::string name = iter->path().leaf().string();

    // search for our pattern
    if (regex_match(name, searchedFile)) {
      // detected a file name match
      searchResults.push_back(iter->path());
    }
  }
  return searchResults;
}

/*
 *
 */
vector<fs::path> checkPathPrefix(vector<fs::path> fileFullPaths,
    string prefix) {
  // placeholder for the search result
  vector<fs::path> prefixCheckedResults { };

  if (prefix.empty()) {
    prefixCheckedResults = fileFullPaths;
  }

  boost::regex searchedPrefix { prefix };

  for (fs::path filePath : fileFullPaths) {
    if (regex_match(getPathLastSegment(filePath).string(), searchedPrefix)) {
      prefixCheckedResults.push_back(filePath);
    }
  }

  return prefixCheckedResults;
}

fs::path selectFileInVector(vector<fs::path> checkedFiles) {
  fs::path selectedFile { };
  if (checkedFiles.size() == 1) {
    selectedFile = checkedFiles.front();
  }
  if (checkedFiles.size() > 1) {
    stringstream errorBuffer;
    errorBuffer << "Multiple configuration files detected in one path location";
    throw ElementsException(errorBuffer.str());
  }
  return selectedFile;
}

/*
 * In this search, we first search recursively the file into the different directories provided
 * by the path. When a file which match the provided name is found, the prefix (if provided) is checked.
 * If it does not match, the search for another file match continues.
 */
fs::path searchConfFileRecursivelyInDir(string directoryName,
    string searchedFileName, fs::path prefixPath) {

  vector<fs::path> filesWithGoodName = searchFileRecursivelyInDir(directoryName,
      searchedFileName);

  vector<fs::path> filesWithGoodNameAndPrefix = checkPathPrefix(
      filesWithGoodName, prefixPath.string());
  fs::path selectedFile { };
  try {
    selectedFile = selectFileInVector(filesWithGoodNameAndPrefix);
  } catch (const ElementsException & e) {
    stringstream errorBuffer;
    errorBuffer << "Conf file: " << searchedFileName
        << " found more than once in directory " << directoryName;

    throw ElementsException(errorBuffer.str());
  }
  return selectedFile;
}

/*
 * Iterate over the different directories included in the path and call
 * searchRecursivelyInDir(...) for each of them
 */
fs::path searchConfFileInPathVariable(std::string fileName,
    std::string pathPrefix, std::string configurationExtension,
    std::string pathConfEnvVariable) {

  // Placeholder for the to-be-returned search result
  fs::path searchResult { };

  // Get the wanted configuration filename with correct extension
  fs::path fileNameWithNativeExtention { fileName };
  string searchedFileName { fileNameWithNativeExtention.replace_extension(
      configurationExtension).string() };

  // get the path from the environment variable
  std::string composedPath = getenv(pathConfEnvVariable.c_str());

  // Tokenize the path elements
  vector<string> pathElements;
  boost::split(pathElements, composedPath, boost::is_any_of(";:"));

  // Loop over all path elements
  for (string pathElement : pathElements) {
    // Check if directory exists
    if (fs::exists(pathElement) && fs::is_directory(pathElement)) {
      // loop recursively inside directory
      try {
        searchResult = searchConfFileRecursivelyInDir(pathElement,
            searchedFileName, pathPrefix);
      } catch (const ElementsException & e) {
        throw ElementsException(e.what());
      }
      if (!searchResult.empty()) {
        break;
      }
    }
  }
  return searchResult;
}

//fs::path searchRecursivelyInDir(string directoryName,
//    string searchedFileName, fs::path prefixPath) {
//
//   // placeholder for the search result
//  fs::path searchResult { };
//  // search prefix in the correct format
//  boost::regex searchedPrefix { prefixPath.string() };
//  // has the conf file already been found in some other location?
//  bool resultAlreadyFound = false;
//
//  // Iterate over all files and sub-directories
//  for (fs::recursive_directory_iterator iter(directoryName), end; iter != end;
//      ++iter) {
//
//    // Get the name of a directory element (file or directory itself)
//    std::string name = iter->path().leaf().string();
//
//    // prepare an error string buffer in case of a double detections
//    stringstream errorBuffer;
//    errorBuffer << "Conf file: " << searchedFileName
//        << " found more than once in directory " << directoryName;
//
//    // search for our pattern
//    if (regex_match(name, searchedFileName)) {
//      // detected a file name match
//      // ... case without prefix
//      if (prefixPath.empty()) {
//        // Check if it is the first detection
//        if (resultAlreadyFound) {
//          throw ElementsException(errorBuffer.str());
//        }
//        searchResult = iter->path();
//        resultAlreadyFound = true;
//
//      } else {  // ... case with prefix
//        string detectedFileLastPathSegment = getPathLastSegment(
//            iter->path().parent_path()).string();
//        if (regex_match(detectedFileLastPathSegment, searchedPrefix)) {
//          if (resultAlreadyFound) {
//            throw ElementsException(errorBuffer.str());
//          }
//          searchResult = iter->path();
//          resultAlreadyFound = true;
//
//        }
//      }
//    }
//  }
//  return searchResult;
//}
//
