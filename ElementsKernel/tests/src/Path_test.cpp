/**
 * @file Path_test.cpp
 *
 * Created on: Dec 4, 2013
 *     Author: Pierre Dubath
 */

#include "ElementsKernel/Path.h"          // header to test

#include <string>                         // for std::string
#include <vector>                         // for std::vector
#include <algorithm>                      // for for_each, transform

#include <boost/test/unit_test.hpp>       // for boost unit test macros
#include <boost/filesystem.hpp>           // for boost::filesystem
#include <boost/filesystem/fstream.hpp>   // for ofstream


#include <ElementsKernel/Temporary.h>     // for TempDir

namespace fs = boost::filesystem;

using std::string;
using std::vector;

using Elements::TempDir;


//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

struct Path_Fixture {

  TempDir m_top_dir;
  vector<fs::path> m_item_list;

  Path_Fixture(): m_top_dir{ "Path_test-%%%%%%%" } {

    using std::for_each;

    m_item_list.push_back(m_top_dir.path() / "test1");
    m_item_list.push_back(m_top_dir.path() / "test1" / "foo");
    m_item_list.push_back(m_top_dir.path() / "test2");
    m_item_list.push_back(m_top_dir.path() / "test3");

    for_each(m_item_list.cbegin(), m_item_list.cend(),
        [](fs::path p) {
        fs::create_directory(p);
    });

  }

  ~Path_Fixture() {

  }

};

BOOST_AUTO_TEST_SUITE(Path_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(PathConstructor_test) {

  fs::path test_path {"toto/titi"};

  BOOST_CHECK(test_path.is_relative());
  BOOST_CHECK(test_path.filename() == "titi");
  BOOST_CHECK(test_path.parent_path() == "toto");

  string test_str {"toto/tutu"};
  fs::path test_path2 { test_str };

  BOOST_CHECK(test_path2.is_relative());
  BOOST_CHECK(test_path2.string() == test_path2);

  fs::path test_path3 {};
  BOOST_CHECK(test_path3.string() == "");
  BOOST_CHECK(test_path3.filename() == "");
  BOOST_CHECK(test_path3.parent_path() == "");

}

BOOST_FIXTURE_TEST_CASE(getFromLocations_test, Path_Fixture) {

  using Elements::Path::getPathFromLocations;
  using std::endl;

  fs::path that_file = getPathFromLocations("Bla", m_item_list);

  BOOST_CHECK(that_file.filename() == "");

  fs::path that_dir = getPathFromLocations("foo", m_item_list);
  BOOST_CHECK(that_dir.filename() != "");
  BOOST_CHECK(fs::exists(that_dir));

  fs::path f_path {m_top_dir.path() / "test2" / "bar"};

  fs::ofstream f{f_path.string()};
  f << "test text" << endl;
  f.close();

  fs::path that_file2 = getPathFromLocations("bar", m_item_list);
  BOOST_CHECK(that_file2.filename() != "");
  BOOST_CHECK(fs::exists(that_file2));
  BOOST_CHECK(fs::is_regular_file(that_file2));

}

BOOST_FIXTURE_TEST_CASE(getFromStringLocations_test, Path_Fixture) {

  using Elements::Path::getPathFromLocations;
  using std::endl;
  using std::transform;

  vector<string> str_item_list(m_item_list.size());

  transform(m_item_list.cbegin(), m_item_list.cend(),
      str_item_list.begin(),
      [](fs::path p){
      return p.string();
  });

  fs::path that_file = getPathFromLocations("Bla", str_item_list);

  BOOST_CHECK(that_file.filename() == "");

  fs::path that_dir = getPathFromLocations("foo", str_item_list);
  BOOST_CHECK(that_dir.filename() != "");
  BOOST_CHECK(fs::exists(that_dir));

  fs::path f_path {m_top_dir.path() / "test2" / "bar"};

  fs::ofstream f{f_path.string()};
  f << "test text" << endl;
  f.close();

  fs::path that_file2 = getPathFromLocations("bar", str_item_list);
  BOOST_CHECK(that_file2.filename() != "");
  BOOST_CHECK(fs::exists(that_file2));
  BOOST_CHECK(fs::is_regular_file(that_file2));

}


BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------
