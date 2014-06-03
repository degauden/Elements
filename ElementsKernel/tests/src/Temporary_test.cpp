/**
 * @file Path_test.cpp
 *
 * Created on: Dec 4, 2013
 *     Author: Pierre Dubath
 */
#include <string>
#include <vector>
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

#include "ElementsKernel/Temporary.h"
#include "ElementsKernel/ElementsException.h"

using namespace std;

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

struct Temporary_Fixture {

  Temporary_Fixture() {
    // setup
  }
  ~Temporary_Fixture() {
    // teardown
  }
};

BOOST_AUTO_TEST_SUITE (Temporary_test)

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(AutoDestruct_test, Temporary_Fixture) {

  fs::path test_path;
  fs::path test_file_path ;

  {
	TempDir One;
	test_path = One.path();
	BOOST_CHECK(fs::exists(test_path));

	TempFile Two;
	test_file_path = Two.path();
	BOOST_CHECK(fs::exists(test_file_path));
  }

  BOOST_CHECK(!fs::exists(test_path));
  BOOST_CHECK(!fs::exists(test_file_path));

  BOOST_CHECK(true);

}


BOOST_AUTO_TEST_SUITE_END ()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------
