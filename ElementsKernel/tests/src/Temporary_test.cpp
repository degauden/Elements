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
#include <boost/filesystem/fstream.hpp>

namespace fs = boost::filesystem;

#include "ElementsKernel/Temporary.h"
#include "ElementsKernel/Exception.h"
#include "ElementsKernel/System.h"     // for getEnv, setEnv, unSetEnv

using namespace std;

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

struct Temporary_Fixture {

  Elements::TempDir m_top_dir{"Temporary_test-%%%%%%%"};

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
	Elements::TempDir one;
	test_path = one.path();
	BOOST_CHECK(fs::exists(test_path));

	Elements::TempFile two;
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
  Elements::TempDir three;
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

BOOST_FIXTURE_TEST_CASE(TempEnv_test, Temporary_Fixture) {

  using Elements::System::getEnv;
  using Elements::System::setEnv;
  using Elements::System::unSetEnv;

  // test if the global temporary directory exists.
  BOOST_CHECK(fs::exists(m_top_dir.path()));
  fs::path test_tmpdir = m_top_dir.path()/"tmpdir";
  fs::create_directory(test_tmpdir);
  setEnv("TMPDIR", test_tmpdir.c_str(), 1);
  string tmp_env_val = getEnv("TMPDIR");
  // test that the variable is actually set in the environment
  // of the process
  BOOST_CHECK(tmp_env_val == test_tmpdir.string());

  // create a new temporary directory that should be rooted at the
  // value of the TMPDIR directory.

  Elements::TempDir new_one;

  // test that the new tmp directory has been created in the right
  // directory (in $TMPDIR)
  BOOST_CHECK(new_one.path().parent_path() == test_tmpdir);

  // remove the environment variable
  unSetEnv("TMPDIR");
  // check that it is gone
  BOOST_CHECK(getEnv("TMPDIR") == "UNKNOWN");
  BOOST_CHECK(fs::exists(test_tmpdir));
}

BOOST_AUTO_TEST_SUITE_END ()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------
