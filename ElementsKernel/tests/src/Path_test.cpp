/**
 * @file Path_test.cpp
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

#include "ElementsKernel/Path.h"  // header to test

#include <algorithm>  // for for_each, transform
#include <string>     // for std::string
#include <vector>     // for std::vector

#include <boost/filesystem.hpp>          // for boost::filesystem
#include <boost/filesystem/fstream.hpp>  // for ofstream
#include <boost/test/unit_test.hpp>      // for boost unit test macros

#include "ElementsKernel/Temporary.h"  // for TempDir

using std::string;
using std::vector;

using boost::filesystem::exists;
using boost::filesystem::is_regular;

namespace Elements {

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

struct Path_Fixture {

  TempDir            m_top_dir;
  vector<Path::Item> m_item_list;
  vector<Path::Item> m_file_list;
  vector<Path::Item> m_directory_list;

  static void create_test_file(const Path::Item& p) {

    boost::filesystem::ofstream f{p.string()};
    f << "test text" << std::endl;
    f.close();
  }

  Path_Fixture() : m_top_dir{"Path_test-%%%%%%%"} {

    using boost::filesystem::create_directory;
    using std::for_each;

    m_item_list.emplace_back(m_top_dir.path() / "test1");
    m_item_list.emplace_back(m_top_dir.path() / "test1" / "foo");
    m_item_list.emplace_back(m_top_dir.path() / "test2");
    m_item_list.emplace_back(m_top_dir.path() / "test3");
    m_item_list.emplace_back(m_top_dir.path() / "test4");
    m_item_list.emplace_back(m_top_dir.path() / "test5");
    m_item_list.emplace_back(m_top_dir.path() / "test6");

    for_each(m_item_list.cbegin(), m_item_list.cend(), [](Path::Item p) {
      create_directory(p);
    });

    m_file_list.emplace_back(m_top_dir.path() / "test1" / "foo" / "e1e2");
    m_directory_list.emplace_back(m_top_dir.path() / "test1" / "foo");

    m_file_list.emplace_back(m_top_dir.path() / "test3" / "e1e2");
    m_directory_list.emplace_back(m_top_dir.path() / "test3");

    m_file_list.emplace_back(m_top_dir.path() / "test4" / "e1e2");
    m_directory_list.emplace_back(m_top_dir.path() / "test4");

    create_directory(m_top_dir.path() / "test1" / "sub");
    create_directory(m_top_dir.path() / "test5" / "sub");

    m_file_list.emplace_back(m_top_dir.path() / "test1" / "sub" / "d1d2");
    m_directory_list.emplace_back(m_top_dir.path() / "test1" / "sub");

    m_file_list.emplace_back(m_top_dir.path() / "test5" / "sub" / "d1d2");
    m_directory_list.emplace_back(m_top_dir.path() / "test5" / "sub");

    m_file_list.emplace_back(m_top_dir.path() / "test6" / "d1d2");
    m_directory_list.emplace_back(m_top_dir.path() / "test6");

    for_each(m_file_list.cbegin(), m_file_list.cend(), [](Path::Item p) {
      create_test_file(p);
    });
  }

  ~Path_Fixture() {}
};

BOOST_AUTO_TEST_SUITE(Path_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(PathConstructor_test) {

  Path::Item test_path{"toto/titi"};

  BOOST_CHECK(test_path.is_relative());
  BOOST_CHECK(test_path.filename() == "titi");
  BOOST_CHECK(test_path.parent_path() == "toto");

  string     test_str{"toto/tutu"};
  Path::Item test_path2{test_str};

  BOOST_CHECK(not test_path2.empty());

  BOOST_CHECK(test_path2.is_relative());
  BOOST_CHECK(test_path2.string() == test_path2);

  Path::Item test_path3{};
  BOOST_CHECK(test_path3.empty());

  BOOST_CHECK(test_path3.string() == "");
  BOOST_CHECK(test_path3.filename() == "");
  BOOST_CHECK(test_path3.parent_path() == "");
}

BOOST_FIXTURE_TEST_CASE(getFromLocations_test, Path_Fixture) {

  using Path::getPathFromLocations;

  auto that_file = getPathFromLocations("Bla", m_item_list);

  BOOST_CHECK(that_file.filename() == "");

  auto that_dir = getPathFromLocations("foo", m_item_list);
  BOOST_CHECK(that_dir.filename() != "");
  BOOST_CHECK(exists(that_dir));

  Path::Item f_path{m_top_dir.path() / "test2" / "bar"};

  create_test_file(f_path);

  auto that_file2 = getPathFromLocations("bar", m_item_list);
  BOOST_CHECK(that_file2.filename() != "");
  BOOST_CHECK(exists(that_file2));
  BOOST_CHECK(is_regular_file(that_file2));
}

BOOST_FIXTURE_TEST_CASE(getAllFromLocations_test, Path_Fixture) {

  using Path::getAllPathFromLocations;

  vector<Path::Item> file_list = getAllPathFromLocations("e1e2", m_item_list);

  BOOST_CHECK(file_list.size() == 3);

  vector<Path::Item> ref_file_list;
  ref_file_list.emplace_back(m_top_dir.path() / "test1" / "foo" / "e1e2");
  ref_file_list.emplace_back(m_top_dir.path() / "test3" / "e1e2");
  ref_file_list.emplace_back(m_top_dir.path() / "test4" / "e1e2");

  BOOST_CHECK_EQUAL_COLLECTIONS(file_list.cbegin(), file_list.cend(), ref_file_list.cbegin(), ref_file_list.cend());

  vector<Path::Item> file_list2 = getAllPathFromLocations("sub/d1d2", m_item_list);

  BOOST_CHECK(file_list2.size() == 2);

  vector<Path::Item> ref_file_list2;
  ref_file_list2.emplace_back(m_top_dir.path() / "test1" / "sub" / "d1d2");
  ref_file_list2.emplace_back(m_top_dir.path() / "test5" / "sub" / "d1d2");

  BOOST_CHECK_EQUAL_COLLECTIONS(file_list2.cbegin(), file_list2.cend(), ref_file_list2.cbegin(), ref_file_list2.cend());
}

BOOST_FIXTURE_TEST_CASE(getFromStringLocations_test, Path_Fixture) {

  using Path::getPathFromLocations;
  using std::transform;

  vector<string> str_item_list(m_item_list.size());

  transform(m_item_list.cbegin(), m_item_list.cend(), str_item_list.begin(), [](Path::Item p) {
    return p.string();
  });

  auto that_file = getPathFromLocations("Bla", str_item_list);

  BOOST_CHECK(that_file.filename() == "");

  auto that_dir = getPathFromLocations("foo", str_item_list);
  BOOST_CHECK(that_dir.filename() != "");
  BOOST_CHECK(exists(that_dir));

  Path::Item f_path{m_top_dir.path() / "test2" / "bar"};

  create_test_file(f_path);

  auto that_file2 = getPathFromLocations("bar", str_item_list);
  BOOST_CHECK(that_file2.filename() != "");
  BOOST_CHECK(exists(that_file2));
  BOOST_CHECK(is_regular_file(that_file2));
}

BOOST_FIXTURE_TEST_CASE(getPathFromEnvVariable_test, Path_Fixture) {

  using Path::getPathFromEnvVariable;

  auto env = TempEnv();

  env["THAT_PATH"] = Path::join(m_directory_list);

  auto foobar_path = getPathFromEnvVariable("foobar", "THAT_PATH");

  BOOST_CHECK(foobar_path.empty());

  auto e1e2_path = getPathFromEnvVariable("e1e2", "THAT_PATH");

  BOOST_CHECK_EQUAL(e1e2_path, m_top_dir.path() / "test1" / "foo" / "e1e2");
}

BOOST_AUTO_TEST_CASE(JoinPath_test) {

  using Path::joinPath;

  const vector<string> path_list{"/toto", "titi", "./tutu"};

  BOOST_CHECK(joinPath(path_list) == "/toto:titi:./tutu");

  const vector<string> path_list2{"", "/toto", "titi", "./tutu"};

  BOOST_CHECK(joinPath(path_list2) == ":/toto:titi:./tutu");

  const vector<string> path_list3{"/toto", "titi", "./tutu", ""};

  BOOST_CHECK(joinPath(path_list3) == "/toto:titi:./tutu:");
}

BOOST_AUTO_TEST_CASE(Join_test) {

  using Path::join;

  const vector<string> path_list{"/toto", "titi", "./tutu"};

  BOOST_CHECK(join(path_list) == "/toto:titi:./tutu");

  const vector<string> path_list2{"", "/toto", "titi", "./tutu"};

  BOOST_CHECK(join(path_list2) == ":/toto:titi:./tutu");

  const vector<string> path_list3{"/toto", "titi", "./tutu", ""};

  BOOST_CHECK(join(path_list3) == "/toto:titi:./tutu:");
}

BOOST_AUTO_TEST_CASE(SplitPath_test) {

  using Path::splitPath;

  const vector<Path::Item> path_list{"/toto", "titi", "./tutu"};
  const string             path_string{"/toto:titi:./tutu"};
  BOOST_CHECK(splitPath(path_string) == path_list);

  const vector<Path::Item> path_list2{"", "/toto", "titi", "./tutu"};
  const string             path_string2{":/toto:titi:./tutu"};
  BOOST_CHECK(splitPath(path_string2) == path_list2);

  const vector<Path::Item> path_list3{"/toto", "titi", "./tutu", ""};
  const string             path_string3{"/toto:titi:./tutu:"};
  BOOST_CHECK(splitPath(path_string3) == path_list3);
}

BOOST_AUTO_TEST_CASE(MultiPathAppend_test) {

  using Path::multiPathAppend;

  const vector<string> locations{"loc1", "/loc2", "./loc3"};
  const vector<string> suffixes{"bin", "scripts"};

  const vector<string> ref_paths{"loc1/bin",      "loc1/scripts", "/loc2/bin",
                                 "/loc2/scripts", "./loc3/bin",   "./loc3/scripts"};

  const vector<Path::Item> full_paths = multiPathAppend(locations, suffixes);

  BOOST_CHECK(full_paths.size() == 6);

  vector<string> full_path_strings(full_paths.size());

  std::transform(full_paths.cbegin(), full_paths.cend(), full_path_strings.begin(), [](Path::Item p) {
    return p.string();
  });

  BOOST_CHECK(ref_paths == full_path_strings);
}

BOOST_AUTO_TEST_CASE(RemoveDuplicates_test) {

  using Path::removeDuplicates;

  const vector<string> locations{"/usr/bin",       "/usr/local/bin", "/usr/bin",      "/opt/bin",
                                 "/opt/local/bin", "/usr/bin",       "/usr/local/bin"};

  const vector<string> unique_locations{"/usr/bin", "/usr/local/bin", "/opt/bin", "/opt/local/bin"};
  vector<Path::Item>   unique_paths;

  for (const auto& l : unique_locations) {
    unique_paths.emplace_back(Path::Item(l));
  }

  BOOST_CHECK(removeDuplicates(locations) == unique_paths);

  vector<Path::Item> paths;

  for (const auto& l : locations) {
    paths.emplace_back(Path::Item(l));
  }

  BOOST_CHECK(removeDuplicates(paths) == unique_paths);
}

BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------

}  // namespace Elements
