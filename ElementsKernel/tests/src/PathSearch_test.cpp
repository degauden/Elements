/**
 * @file Path_test.cpp
 *
 * Created on: Dec 4, 2013
 *     Author: Pierre Dubath
 */
#include <string>
#include <vector>
#include <cstdlib>
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;

#include "ElementsKernel/PathSearch.h"
#include "ElementsKernel/Exception.h"

using namespace std;

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

struct Path_Fixture {

  string m_env_variable_name = "ELEMENTS_CONF_PATH";
  // store the original value of the environment variable to set it again correctly after the test
  const char * m_tmp_char_ptr = ::getenv(m_env_variable_name.c_str());

  string m_root_path = ::getenv("ELEMENTSKERNELROOT");
  string m_string_full_path = m_root_path + "/tests/data/PathSearch/";
  fs::path m_full_path { m_string_full_path };

  string m_full_path_1 = m_root_path
      + "/tests/data/PathSearch/dirForTestingRecursiveSearch";
  string m_full_path_2 = m_root_path + "/tests/data/PathSearch//ElementsKernel";
  string m_multiple_path = "/opt/local/bin:" + m_full_path_1 + ":" + m_full_path_2
      + ":/bin";

  Path_Fixture() {
    // setup
  }
  ~Path_Fixture() {
    // teardown
    if (m_tmp_char_ptr != nullptr) {
      ::setenv(m_env_variable_name.c_str(), m_tmp_char_ptr, 1);
    }
  }
};

BOOST_AUTO_TEST_SUITE (Path_test)

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(simple_file_search, Path_Fixture) {

  string file_name { "MockFile_up.conf" };

  fs::path expectedFullPath = m_full_path / fs::path(file_name);
  fs::path actualFullPath =
      Elements::pathSearch(file_name, m_full_path, Elements::SearchType::Local).at(0);
  BOOST_CHECK(actualFullPath == expectedFullPath);
  actualFullPath = Elements::pathSearch(file_name, m_full_path, Elements::SearchType::Recursive).at(
      0);
  BOOST_CHECK(actualFullPath == expectedFullPath);

}

BOOST_FIXTURE_TEST_CASE(simple_directory_search, Path_Fixture) {

  string file_name { "ElementsKernel" };

  fs::path expectedFullPath = m_full_path / fs::path(file_name);
  fs::path actualFullPath =
      Elements::pathSearch(file_name, m_full_path, Elements::SearchType::Local).at(0);
  BOOST_CHECK(actualFullPath == expectedFullPath);
  BOOST_CHECK(
      Elements::pathSearch(file_name, m_full_path, Elements::SearchType::Recursive).size() == 2);

}

BOOST_FIXTURE_TEST_CASE(recursive_file_search, Path_Fixture) {

  string file_name { "MockFile_down.conf" };

  // should not be found on a Local search, pathSearch returns a vector of size = 0
  BOOST_CHECK(Elements::pathSearch(file_name, m_full_path, Elements::SearchType::Local).size() == 0);

  fs::path expectedFullPath = m_full_path
      / fs::path("dirForTestingRecursiveSearch") / fs::path(file_name);
  // only one MockFile_down.conf in first position of the path vector
  fs::path actualFullPath = Elements::pathSearch(file_name, m_full_path,
      Elements::SearchType::Recursive).at(0);
  BOOST_CHECK(actualFullPath == expectedFullPath);
}

BOOST_FIXTURE_TEST_CASE(recursive_search_string_interface, Path_Fixture) {

  string file_name { "MockFile_down.conf" };

  // should not be found on a Local search, pathSearch returns a vector of size = 0
  BOOST_CHECK(
      Elements::pathSearch(file_name, m_full_path.string(), Elements::SearchType::Local).size() == 0);

  fs::path expectedFullPath = m_full_path
      / fs::path("dirForTestingRecursiveSearch") / fs::path(file_name);
  // only one MockFile_down.conf in first position of the path vector
  string actualFullPath = Elements::pathSearch(file_name, m_full_path.string(),
      Elements::SearchType::Recursive).at(0);
  BOOST_CHECK(actualFullPath == expectedFullPath.string());
}

BOOST_FIXTURE_TEST_CASE(duplicate_file_search, Path_Fixture) {

  string file_name { "MockFile_replicate.conf" };

  // should not be found on a Local search, pathSearch returns a vector of size = 0
  BOOST_CHECK(Elements::pathSearch(file_name, m_full_path, Elements::SearchType::Local).size() == 0);

  // two MockFile_replicate.conf should be found on a recursive search
  BOOST_CHECK(
      Elements::pathSearch(file_name, m_full_path.string(), Elements::SearchType::Recursive).size()
          == 2);

}

//----------------------------------------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(searchFileInEnvVariable_single_file, Path_Fixture) {

  setenv(m_env_variable_name.c_str(), m_multiple_path.c_str(), 1);
  string file { "MockFile_in_ElementsKernel_up.conf" };

  vector<fs::path> actualFullPathVector = Elements::pathSearchInEnvVariable(file,
      m_env_variable_name);

  BOOST_CHECK(actualFullPathVector.size() == 1);

  fs::path expectedFullPath = m_full_path / fs::path {
      "ElementsKernel/MockFile_in_ElementsKernel_up.conf" };

  BOOST_CHECK(expectedFullPath == actualFullPathVector.at(0));
}

BOOST_FIXTURE_TEST_CASE(searchFileInEnvVariable_multiple_file, Path_Fixture) {

  setenv(m_env_variable_name.c_str(), m_multiple_path.c_str(), 1);
  string file { "MockFile_replicate.conf" };

  vector<fs::path> actualFullPathVector = Elements::pathSearchInEnvVariable(file,
      m_env_variable_name);

  BOOST_CHECK(actualFullPathVector.size() == 2);

  fs::path expectedFullPath_1 = m_full_path_1 / fs::path {
      "ElementsKernel/MockFile_replicate.conf" };
  fs::path expectedFullPath_2 = m_full_path_2 / fs::path {
      "MockFile_replicate.conf" };

  if (expectedFullPath_1 != actualFullPathVector.at(0)) {
    BOOST_CHECK(expectedFullPath_1 == actualFullPathVector.at(1));
    BOOST_CHECK(expectedFullPath_2 == actualFullPathVector.at(0));
  } else {
    BOOST_CHECK(expectedFullPath_1 == actualFullPathVector.at(0));
    BOOST_CHECK(expectedFullPath_2 == actualFullPathVector.at(1));
  }
}

/*
 * Check whether the first file appearing in one of the path elements
 * is taken first
 */
BOOST_FIXTURE_TEST_CASE(searchFileInEnvVariable_order, Path_Fixture) {

  // first order
  string multiple_path = "/opt/local/bin:" + m_full_path_1 + ":" + m_full_path_2
      + ":/bin";
  setenv(m_env_variable_name.c_str(), multiple_path.c_str(), 1);
  string file_1 { "MockFile_replicate.conf" };

  vector<fs::path> actualFullPathVector = Elements::pathSearchInEnvVariable(file_1,
      m_env_variable_name);
  fs::path expectedFullPath = m_full_path_1 / fs::path {
      "ElementsKernel/MockFile_replicate.conf" };
  BOOST_CHECK(expectedFullPath == actualFullPathVector.at(0));

  // Opposite order
  multiple_path = "/opt/local/bin:" + m_full_path_2 + ":" + m_full_path_1 + ":/bin";
  setenv(m_env_variable_name.c_str(), multiple_path.c_str(), 1);
  string file_2 { "MockFile_replicate.conf" };

  actualFullPathVector = Elements::pathSearchInEnvVariable(file_2, m_env_variable_name);
  expectedFullPath = m_full_path_2 / fs::path { "MockFile_replicate.conf" };
  BOOST_CHECK(expectedFullPath == actualFullPathVector.at(0));
}

BOOST_FIXTURE_TEST_CASE(searchFileInEnvVariable_file_not_found, Path_Fixture) {

  setenv(m_env_variable_name.c_str(), m_multiple_path.c_str(), 1);
  string file { "NonExistentFile.conf" };

  vector<fs::path> actualFullPathVector = Elements::pathSearchInEnvVariable(file,
      m_env_variable_name);

  BOOST_CHECK(actualFullPathVector.size() == 0);
}

BOOST_FIXTURE_TEST_CASE(searchFileInEnvVariable_Env_Variable_Undefine, Path_Fixture) {

  unsetenv(m_env_variable_name.c_str());
  string file { "MockFile_replicate.conf" };

  vector<fs::path> actualFullPathVector = Elements::pathSearchInEnvVariable(file,
          m_env_variable_name);
  
  BOOST_CHECK(actualFullPathVector.empty());
}

BOOST_AUTO_TEST_SUITE_END ()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------
