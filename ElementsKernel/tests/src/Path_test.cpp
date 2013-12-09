/**
 * @file Path_test.cpp
 *
 * Created on: Dec 4, 2013
 *     Author: Pierre Dubath
 */
#include <string>
#include <vector>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;

#include "ElementsKernel/Path.h"
#include "ElementsKernel/ElementsException.h"

using namespace std;

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

struct Path_Fixture {


  string rootPath = getenv("ELEMENTSKERNELROOT");
    string stringFullPath = rootPath + "/tests/conf/";
    fs::path fullPath { stringFullPath };

  Path_Fixture() {
    // setup
  }
  ~Path_Fixture() {
    // teardown
  }
};

BOOST_AUTO_TEST_SUITE (Path_test)

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(simple_file_search, Path_Fixture) {

  string fileName { "MockFile_up.conf" };

  fs::path expectedFullPath = fullPath / fs::path(fileName);
  fs::path actualFullPath =
      pathSearch(fileName, fullPath, SearchType::Local).at(0);
  BOOST_CHECK(actualFullPath == expectedFullPath);
  actualFullPath =
      pathSearch(fileName, fullPath, SearchType::Recursive).at(0);
  BOOST_CHECK(actualFullPath == expectedFullPath);

}

BOOST_FIXTURE_TEST_CASE(simple_directory_search, Path_Fixture) {

  string fileName { "ElementsKernel" };

  fs::path expectedFullPath = fullPath / fs::path(fileName);
  fs::path actualFullPath =
      pathSearch(fileName, fullPath, SearchType::Local).at(0);
  BOOST_CHECK(actualFullPath == expectedFullPath);
  BOOST_CHECK(pathSearch(fileName, fullPath, SearchType::Recursive).size() == 4);

}


BOOST_FIXTURE_TEST_CASE(recursive_file_search, Path_Fixture) {

  string fileName { "MockFile_down.conf" };

  // should not be found on a Local search, pathSearch returns a vector of size = 0
  BOOST_CHECK(pathSearch(fileName, fullPath, SearchType::Local).size() == 0);

  fs::path expectedFullPath = fullPath / fs::path("dirForTestingRecursiveSearch") / fs::path(fileName);
  // only one MockFile_down.conf in first position of the path vector
  fs::path actualFullPath =
       pathSearch(fileName, fullPath, SearchType::Recursive).at(0);
  BOOST_CHECK(actualFullPath == expectedFullPath);
}

BOOST_FIXTURE_TEST_CASE(recursive_search_string_interface, Path_Fixture) {

  string fileName { "MockFile_down.conf" };

  // should not be found on a Local search, pathSearch returns a vector of size = 0
  BOOST_CHECK(pathSearch(fileName, fullPath.string(), SearchType::Local).size() == 0);

  fs::path expectedFullPath = fullPath / fs::path("dirForTestingRecursiveSearch") / fs::path(fileName);
  // only one MockFile_down.conf in first position of the path vector
  string actualFullPath =
       pathSearch(fileName, fullPath.string(), SearchType::Recursive).at(0);
  BOOST_CHECK(actualFullPath == expectedFullPath.string());
}

BOOST_FIXTURE_TEST_CASE(duplicate_file_search, Path_Fixture) {

  string fileName { "MockFile_replicate.conf" };

  // should not be found on a Local search, pathSearch returns a vector of size = 0
  BOOST_CHECK(pathSearch(fileName, fullPath, SearchType::Local).size() == 0);

  // two MockFile_replicate.conf should be found on a recursive search
  BOOST_CHECK(pathSearch(fileName, fullPath.string(), SearchType::Recursive).size() == 2);

}

BOOST_AUTO_TEST_SUITE_END ()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------
