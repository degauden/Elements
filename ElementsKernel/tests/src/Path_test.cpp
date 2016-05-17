/**
 * @file Path_test.cpp
 *
 * Created on: Dec 4, 2013
 *     Author: Pierre Dubath
 */

#include "ElementsKernel/Path.h"          // header to test

#include <string>                         // for std::string
#include <vector>                         // for std::vector

#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;


using std::string;
using std::vector;


//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

struct Path_Fixture {


  Path_Fixture() {
    // setup
  }
  ~Path_Fixture() {

  }

};

BOOST_AUTO_TEST_SUITE(Path_test)

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(simple_path_test, Path_Fixture) {


}


BOOST_AUTO_TEST_CASE(PathConstructor_test) {

  fs::path test_path {"toto/titi"};

  BOOST_CHECK(test_path.is_relative());
  BOOST_CHECK(test_path.filename() == "titi");
  BOOST_CHECK(test_path.parent_path() == "toto");

  string test_str {"toto/tutu"};
  fs::path test_path2 { test_str };

  BOOST_CHECK(test_path2.is_relative());
  BOOST_CHECK(test_path2.string() == test_path2);

}



BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------
