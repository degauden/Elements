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
#include <boost/filesystem/fstream.hpp>

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

  TempDir top_dir{"Temporary_test-%%%%%%%"};

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

  // handle on created path names
  fs::path test_path;
  fs::path test_file_path ;

  {
	// block creation for local variables
	TempDir one;
	test_path = one.path();
	BOOST_CHECK(fs::exists(test_path));

	TempFile two;
	test_file_path = two.path();
	BOOST_CHECK(fs::exists(test_file_path));
  }

  // the items must have been destroy after the
  // closing of the block
  BOOST_CHECK(!fs::exists(test_path));
  BOOST_CHECK(!fs::exists(test_file_path));

  fs::path test2_path ;
  fs::path test2_file_path ;

  {
	TempDir three;
	test2_path = three.path();
	test2_file_path = test2_path / "toto.txt" ;
	BOOST_CHECK(!fs::exists(test2_file_path)) ;
	fs::ofstream ofs(test2_file_path);
	ofs << "test text" << endl;
	ofs.close();
	BOOST_CHECK(fs::exists(test2_file_path)) ;
	BOOST_CHECK(fs::exists(test2_path)) ;
  }

  BOOST_CHECK(!fs::exists(test2_path)) ;
  BOOST_CHECK(!fs::exists(test2_file_path)) ;

}


BOOST_AUTO_TEST_SUITE_END ()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------
