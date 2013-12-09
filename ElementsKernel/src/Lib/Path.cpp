/**
 * @file Path.cpp
 *
 * Created on: Dec 4, 2013
 *     Author: Pierre Dubath
 */
#include <iostream>
#include <sstream>
#include <boost/algorithm/string.hpp>

#include "ElementsKernel/Path.h"
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
vector<T> searchOption(string searched_name, T directory, SearchType search_type) {

  // create a local tmp vector result to avoid multiple return statements
  vector<T> searchResults { };
  switch (search_type) {
  case SearchType::Local:
    searchResults = pathSearch<T, fs::directory_iterator>(searched_name, directory);
    break;
  case SearchType::Recursive:
    searchResults = pathSearch<T, fs::recursive_directory_iterator>(searched_name, directory);
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

