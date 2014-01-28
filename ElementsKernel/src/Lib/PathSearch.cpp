/**
 * @file Path.cpp
 *
 * Created on: Dec 4, 2013
 *     Author: Pierre Dubath
 */
#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>

#include "ElementsKernel/PathSearch.h"
#include "ElementsKernel/ElementsException.h"

namespace fs = boost::filesystem;
using namespace std;

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
    std::string path_like_env_variable) {

  // Placeholder for the to-be-returned search result
  vector<fs::path> search_results { };
  vector<fs::path> single_path_results { };

  // get the multiple path from the environment variable
  char * multiple_path_ptr = getenv(path_like_env_variable.c_str());
  if (multiple_path_ptr == nullptr) {
    stringstream errorBuffer;
    errorBuffer << "Environment variable: " << path_like_env_variable.c_str()
        << " is not defined ! ";
    throw ElementsException(errorBuffer.str());
  }
  string multiple_path = multiple_path_ptr;

  // Tokenize the path elements
  vector<string> path_elements;
  boost::split(path_elements, multiple_path, boost::is_any_of(";:"));

  // Loop over all path elements
  for (string path_element : path_elements) {
    // Check if directory exists
    if (fs::exists(path_element) && fs::is_directory(path_element)) {
      // loop recursively inside directory
      single_path_results = pathSearch(file_name, fs::path { path_element },
          SearchType::Recursive);
      for (fs::path aPath : single_path_results) {
        search_results.push_back(aPath);
      }
    }
  }
  return search_results;
}

