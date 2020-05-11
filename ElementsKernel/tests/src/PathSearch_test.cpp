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
#include "ElementsKernel/Temporary.h"     // for the TempDir class
#include "ElementsKernel/Environment.h"   // for the Environment class
#include "ElementsKernel/Auxiliary.h"     // for getAuxiliaryPath

using std::string;
using std::vector;

using boost::filesystem::path;

namespace Elements {

struct PathSearch_Fixture {

  Environment m_env;
  string m_env_variable_name = "ELEMENTS_CONF_PATH";

  path m_root_path = getAuxiliaryPath("ElementsKernel/tests");
  path m_full_path = m_root_path / "PathSearch/";


  path m_full_path_1 = m_root_path / "PathSearch/dirForTestingRecursiveSearch";
  path m_full_path_2 = m_root_path / "PathSearch//ElementsKernel";
  string m_multiple_path = "/opt/local/bin:" + m_full_path_1.string() + ":" + m_full_path_2.string()
      + ":/bin";

  PathSearch_Fixture() = default;

  ~PathSearch_Fixture() = default;
};

void createTemporaryStructure(const path& top_path) {
  boost::filesystem::create_directories(top_path / "tests" / "data" / "PathSearch");
}


BOOST_AUTO_TEST_SUITE(PathSearch_test)

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(simple_file_search, PathSearch_Fixture) {

  string file_name { "MockFile_up.conf" };

  path expected_full_path = m_full_path / path(file_name);
  path actual_full_path = pathSearch(file_name, m_full_path, SearchType::Local).at(0);
  BOOST_CHECK(actual_full_path == expected_full_path);
  actual_full_path = pathSearch(file_name, m_full_path, SearchType::Recursive).at(
      0);
  BOOST_CHECK(actual_full_path == expected_full_path);

}

BOOST_FIXTURE_TEST_CASE(simple_directory_search, PathSearch_Fixture) {

  string file_name { "ElementsKernel" };

  path expected_full_path = m_full_path / path(file_name);
  path actual_full_path = pathSearch(file_name, m_full_path, SearchType::Local).at(0);
  BOOST_CHECK(actual_full_path == expected_full_path);
  BOOST_CHECK(pathSearch(file_name, m_full_path, SearchType::Recursive).size() == 2);

}

BOOST_FIXTURE_TEST_CASE(recursive_file_search, PathSearch_Fixture) {

  string file_name { "MockFile_down.conf" };

  // should not be found on a Local search, pathSearch returns a vector of size = 0
  BOOST_CHECK(pathSearch(file_name, m_full_path, SearchType::Local).size() == 0);

  path expected_full_path = m_full_path
      / path("dirForTestingRecursiveSearch") / path(file_name);
  // only one MockFile_down.conf in first position of the path vector
  path actual_full_path = pathSearch(file_name, m_full_path, SearchType::Recursive).at(0);
  BOOST_CHECK(actual_full_path == expected_full_path);
}

BOOST_FIXTURE_TEST_CASE(recursive_search_string_interface, PathSearch_Fixture) {

  string file_name { "MockFile_down.conf" };

  // should not be found on a Local search, pathSearch returns a vector of size = 0
  BOOST_CHECK(pathSearch(file_name, m_full_path.string(), SearchType::Local).size() == 0);

  path expected_full_path = m_full_path
      / path("dirForTestingRecursiveSearch") / path(file_name);
  // only one MockFile_down.conf in first position of the path vector
  string actual_full_path = pathSearch(file_name, m_full_path.string(), SearchType::Recursive).at(0);
  BOOST_CHECK(actual_full_path == expected_full_path.string());
}

BOOST_FIXTURE_TEST_CASE(duplicate_file_search, PathSearch_Fixture) {

  string file_name { "MockFile_replicate.conf" };

  // should not be found on a Local search, pathSearch returns a vector of size = 0
  BOOST_CHECK(pathSearch(file_name, m_full_path, SearchType::Local).size() == 0);

  // two MockFile_replicate.conf should be found on a recursive search
  BOOST_CHECK(pathSearch(file_name, m_full_path.string(), SearchType::Recursive).size() == 2);

}

//----------------------------------------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(searchFileInEnvVariable_single_file, PathSearch_Fixture) {

  Environment local;
  local[m_env_variable_name] = m_multiple_path;

  string file { "MockFile_in_ElementsKernel_up.conf" };

  vector<path> actual_full_path_vector = pathSearchInEnvVariable(file, m_env_variable_name);

  BOOST_CHECK(actual_full_path_vector.size() == 1);

  path expected_full_path = m_full_path / path {
      "ElementsKernel/MockFile_in_ElementsKernel_up.conf" };

  BOOST_CHECK(expected_full_path == actual_full_path_vector.at(0));
}

BOOST_FIXTURE_TEST_CASE(searchFileInEnvVariable_multiple_file, PathSearch_Fixture) {

  Environment local;
  local[m_env_variable_name] = m_multiple_path;

  string file { "MockFile_replicate.conf" };

  vector<path> actual_full_path_vector = pathSearchInEnvVariable(file, m_env_variable_name);

  BOOST_CHECK(actual_full_path_vector.size() == 2);

  path expected_full_path_1 = m_full_path_1.string() / path {
      "ElementsKernel/MockFile_replicate.conf" };
  path expected_full_path_2 = m_full_path_2.string() / path {
      "MockFile_replicate.conf" };

  if (expected_full_path_1 != actual_full_path_vector.at(0)) {
    BOOST_CHECK(expected_full_path_1 == actual_full_path_vector.at(1));
    BOOST_CHECK(expected_full_path_2 == actual_full_path_vector.at(0));
  } else {
    BOOST_CHECK(expected_full_path_1 == actual_full_path_vector.at(0));
    BOOST_CHECK(expected_full_path_2 == actual_full_path_vector.at(1));
  }
}

/*
 * Check whether the first file appearing in one of the path elements
 * is taken first
 */
BOOST_FIXTURE_TEST_CASE(searchFileInEnvVariable_order, PathSearch_Fixture) {

  // first order
  string multiple_path = "/opt/local/bin:" + m_full_path_1.string() + ":" + m_full_path_2.string()
      + ":/bin";
  Environment local;
  local[m_env_variable_name] = multiple_path;

  string file_1 { "MockFile_replicate.conf" };

  vector<path> actual_full_path_vector = pathSearchInEnvVariable(file_1, m_env_variable_name);
  path expected_full_path = m_full_path_1.string() / path {
      "ElementsKernel/MockFile_replicate.conf" };
  BOOST_CHECK(expected_full_path == actual_full_path_vector.at(0));

  // Opposite order
  multiple_path = "/opt/local/bin:" + m_full_path_2.string() + ":" + m_full_path_1.string() + ":/bin";
  local[m_env_variable_name] = multiple_path;
  string file_2 { "MockFile_replicate.conf" };

  actual_full_path_vector = pathSearchInEnvVariable(file_2, m_env_variable_name);
  expected_full_path = m_full_path_2.string() / path { "MockFile_replicate.conf" };
  BOOST_CHECK(expected_full_path == actual_full_path_vector.at(0));
}

BOOST_FIXTURE_TEST_CASE(searchFileInEnvVariable_file_not_found, PathSearch_Fixture) {

  Environment local;
  local[m_env_variable_name] = m_multiple_path;

  string file { "NonExistentFile.conf" };

  vector<path> actual_full_path_vector = pathSearchInEnvVariable(file, m_env_variable_name);

  BOOST_CHECK(actual_full_path_vector.size() == 0);
}

BOOST_FIXTURE_TEST_CASE(searchFileInEnvVariable_Env_Variable_Undefine, PathSearch_Fixture) {

  Environment local;
  local.unSet(m_env_variable_name);

  string file { "MockFile_replicate.conf" };

  vector<path> actual_full_path_vector = pathSearchInEnvVariable(file, m_env_variable_name);

  BOOST_CHECK(actual_full_path_vector.empty());
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

  TempDir top_dir {"PathSearch_Recursion_test-%%%%%%%"};
  path top_dir_path = top_dir.path();
  createTemporaryStructure(top_dir_path);
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace Elements
