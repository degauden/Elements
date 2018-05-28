/**
 * @file PathSearch_test.cpp
 *
 * Created on: Dec 4, 2013
 *     Author: Pierre Dubath
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

#include "ElementsKernel/PathSearch.h"

#include <string>                         // for std::string
#include <vector>                         // for std::vector
#include <cstdlib>
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

#include "ElementsKernel/Exception.h"
#include "ElementsKernel/System.h"        // for getEnv
#include "ElementsKernel/Temporary.h"     // For the TempDir class

using std::string;
using std::vector;

using boost::filesystem::path;

using Elements::System::getEnv;
using Elements::System::setEnv;

struct PathSearch_Fixture {

  string m_env_variable_name = "ELEMENTS_CONF_PATH";
  // store the original value of the environment variable to set it again correctly after the test
  const string m_tmp_char_ptr = getEnv(m_env_variable_name);

  string m_root_path = getEnv("ELEMENTSKERNELROOT");
  string m_string_full_path = m_root_path + "/tests/data/PathSearch/";
  path m_full_path { m_string_full_path };

  string m_full_path_1 = m_root_path
      + "/tests/data/PathSearch/dirForTestingRecursiveSearch";
  string m_full_path_2 = m_root_path + "/tests/data/PathSearch//ElementsKernel";
  string m_multiple_path = "/opt/local/bin:" + m_full_path_1 + ":" + m_full_path_2
      + ":/bin";

  PathSearch_Fixture() {
    // setup
  }
  ~PathSearch_Fixture() {
    // teardown
    if (m_tmp_char_ptr.size() != 0) {
      setEnv(m_env_variable_name, m_tmp_char_ptr, 1);
    }
  }
};

void createTemporaryStructure(const path& top_path) {
  boost::filesystem::create_directories(top_path / "tests" / "data" / "PathSearch");
}


BOOST_AUTO_TEST_SUITE(PathSearch_test)

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(simple_file_search, PathSearch_Fixture) {

  string file_name { "MockFile_up.conf" };

  path expectedFullPath = m_full_path / path(file_name);
  path actualFullPath =
      Elements::pathSearch(file_name, m_full_path, Elements::SearchType::Local).at(0);
  BOOST_CHECK(actualFullPath == expectedFullPath);
  actualFullPath = Elements::pathSearch(file_name, m_full_path, Elements::SearchType::Recursive).at(
      0);
  BOOST_CHECK(actualFullPath == expectedFullPath);

}

BOOST_FIXTURE_TEST_CASE(simple_directory_search, PathSearch_Fixture) {

  string file_name { "ElementsKernel" };

  path expectedFullPath = m_full_path / path(file_name);
  path actualFullPath =
      Elements::pathSearch(file_name, m_full_path, Elements::SearchType::Local).at(0);
  BOOST_CHECK(actualFullPath == expectedFullPath);
  BOOST_CHECK(
      Elements::pathSearch(file_name, m_full_path, Elements::SearchType::Recursive).size() == 2);

}

BOOST_FIXTURE_TEST_CASE(recursive_file_search, PathSearch_Fixture) {

  string file_name { "MockFile_down.conf" };

  // should not be found on a Local search, pathSearch returns a vector of size = 0
  BOOST_CHECK(Elements::pathSearch(file_name, m_full_path, Elements::SearchType::Local).size() == 0);

  path expectedFullPath = m_full_path
      / path("dirForTestingRecursiveSearch") / path(file_name);
  // only one MockFile_down.conf in first position of the path vector
  path actualFullPath = Elements::pathSearch(file_name, m_full_path,
      Elements::SearchType::Recursive).at(0);
  BOOST_CHECK(actualFullPath == expectedFullPath);
}

BOOST_FIXTURE_TEST_CASE(recursive_search_string_interface, PathSearch_Fixture) {

  string file_name { "MockFile_down.conf" };

  // should not be found on a Local search, pathSearch returns a vector of size = 0
  BOOST_CHECK(
      Elements::pathSearch(file_name, m_full_path.string(), Elements::SearchType::Local).size() == 0);

  path expectedFullPath = m_full_path
      / path("dirForTestingRecursiveSearch") / path(file_name);
  // only one MockFile_down.conf in first position of the path vector
  string actualFullPath = Elements::pathSearch(file_name, m_full_path.string(),
      Elements::SearchType::Recursive).at(0);
  BOOST_CHECK(actualFullPath == expectedFullPath.string());
}

BOOST_FIXTURE_TEST_CASE(duplicate_file_search, PathSearch_Fixture) {

  string file_name { "MockFile_replicate.conf" };

  // should not be found on a Local search, pathSearch returns a vector of size = 0
  BOOST_CHECK(Elements::pathSearch(file_name, m_full_path, Elements::SearchType::Local).size() == 0);

  // two MockFile_replicate.conf should be found on a recursive search
  BOOST_CHECK(
      Elements::pathSearch(file_name, m_full_path.string(), Elements::SearchType::Recursive).size()
          == 2);

}

//----------------------------------------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(searchFileInEnvVariable_single_file, PathSearch_Fixture) {

  setEnv(m_env_variable_name, m_multiple_path, 1);
  string file { "MockFile_in_ElementsKernel_up.conf" };

  vector<path> actualFullPathVector = Elements::pathSearchInEnvVariable(file,
      m_env_variable_name);

  BOOST_CHECK(actualFullPathVector.size() == 1);

  path expectedFullPath = m_full_path / path {
      "ElementsKernel/MockFile_in_ElementsKernel_up.conf" };

  BOOST_CHECK(expectedFullPath == actualFullPathVector.at(0));
}

BOOST_FIXTURE_TEST_CASE(searchFileInEnvVariable_multiple_file, PathSearch_Fixture) {

  setEnv(m_env_variable_name, m_multiple_path, 1);
  string file { "MockFile_replicate.conf" };

  vector<path> actualFullPathVector = Elements::pathSearchInEnvVariable(file,
      m_env_variable_name);

  BOOST_CHECK(actualFullPathVector.size() == 2);

  path expectedFullPath_1 = m_full_path_1 / path {
      "ElementsKernel/MockFile_replicate.conf" };
  path expectedFullPath_2 = m_full_path_2 / path {
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
BOOST_FIXTURE_TEST_CASE(searchFileInEnvVariable_order, PathSearch_Fixture) {

  // first order
  string multiple_path = "/opt/local/bin:" + m_full_path_1 + ":" + m_full_path_2
      + ":/bin";
  setEnv(m_env_variable_name, multiple_path, 1);
  string file_1 { "MockFile_replicate.conf" };

  vector<path> actualFullPathVector = Elements::pathSearchInEnvVariable(file_1,
      m_env_variable_name);
  path expectedFullPath = m_full_path_1 / path {
      "ElementsKernel/MockFile_replicate.conf" };
  BOOST_CHECK(expectedFullPath == actualFullPathVector.at(0));

  // Opposite order
  multiple_path = "/opt/local/bin:" + m_full_path_2 + ":" + m_full_path_1 + ":/bin";
  setEnv(m_env_variable_name, multiple_path, 1);
  string file_2 { "MockFile_replicate.conf" };

  actualFullPathVector = Elements::pathSearchInEnvVariable(file_2, m_env_variable_name);
  expectedFullPath = m_full_path_2 / path { "MockFile_replicate.conf" };
  BOOST_CHECK(expectedFullPath == actualFullPathVector.at(0));
}

BOOST_FIXTURE_TEST_CASE(searchFileInEnvVariable_file_not_found, PathSearch_Fixture) {

  setEnv(m_env_variable_name, m_multiple_path, 1);
  string file { "NonExistentFile.conf" };

  vector<path> actualFullPathVector = Elements::pathSearchInEnvVariable(file,
      m_env_variable_name);

  BOOST_CHECK(actualFullPathVector.size() == 0);
}

BOOST_FIXTURE_TEST_CASE(searchFileInEnvVariable_Env_Variable_Undefine, PathSearch_Fixture) {

  using Elements::System::unSetEnv;

  unSetEnv(m_env_variable_name);
  string file { "MockFile_replicate.conf" };

  vector<path> actualFullPathVector = Elements::pathSearchInEnvVariable(file,
          m_env_variable_name);

  BOOST_CHECK(actualFullPathVector.empty());
}

BOOST_AUTO_TEST_CASE(PathConstructor_test) {

  path test_path {"toto/titi"};

  BOOST_CHECK(test_path.is_relative());
  BOOST_CHECK(test_path.filename() == "titi");
  BOOST_CHECK(test_path.parent_path() == "toto");

  string test_str {"toto/tutu"};
  path test_path2 { test_str };

  BOOST_CHECK(test_path2.is_relative());
  BOOST_CHECK(test_path2.string() == test_path2);

}

BOOST_AUTO_TEST_CASE(Recursion_test) {

  using Elements::TempDir;

  TempDir top_dir {"PathSearch_Recursion_test-%%%%%%%"};
  path top_dir_path = top_dir.path();
  createTemporaryStructure(top_dir_path);
}

BOOST_AUTO_TEST_SUITE_END()
