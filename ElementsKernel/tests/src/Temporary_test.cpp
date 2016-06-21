/**
 * @file Temporary_test.cpp
 *
 * @date Dec 4, 2013
 * @author Hubert Degaudenzi
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
#include <string>
#include <vector>
#include <cstdlib>
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

namespace fs = boost::filesystem;

#include "ElementsKernel/Temporary.h"  // for Elements::TempDir

#include "ElementsKernel/Exception.h"
#include "ElementsKernel/System.h"     // for getEnv, setEnv, unSetEnv

using Elements::TempDir;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

struct Temporary_Fixture {

  TempDir m_top_dir { "Temporary_test-%%%%%%%" };

  Temporary_Fixture() {
    // setup
  }
  ~Temporary_Fixture() {
    // teardown
  }
};

BOOST_AUTO_TEST_SUITE(Temporary_test)

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(AutoDestruct_test, Temporary_Fixture) {

  // handle on created path names
  fs::path test_path;
  fs::path test_file_path;

  {
    // block creation for local variables
    TempDir one;
    BOOST_CHECK_EQUAL(one.motif(), "");
    test_path = one.path();
    BOOST_CHECK(fs::exists(test_path));

    Elements::TempFile two;
    test_file_path = two.path();
    BOOST_CHECK(fs::exists(test_file_path));
  }

  // the items must have been destroyed after the
  // closing of the block
  BOOST_CHECK(!fs::exists(test_path));
  BOOST_CHECK(!fs::exists(test_file_path));

  fs::path test2_path;
  fs::path test2_file_path;

  {
    using std::endl;

    TempDir three;
    test2_path = three.path();
    test2_file_path = test2_path / "toto.txt";
    BOOST_CHECK(!fs::exists(test2_file_path));
    fs::ofstream ofs(test2_file_path);
    ofs << "test text" << endl;
    ofs.close();
    BOOST_CHECK(fs::exists(test2_file_path));
    BOOST_CHECK(fs::exists(test2_path));
  }

  BOOST_CHECK(!fs::exists(test2_path));
  BOOST_CHECK(!fs::exists(test2_file_path));

  }

BOOST_FIXTURE_TEST_CASE(TempEnv_test, Temporary_Fixture) {

  using std::string;

  using Elements::System::getEnv;
  using Elements::System::setEnv;
  using Elements::System::unSetEnv;

  // test if the global temporary directory exists.
  BOOST_CHECK(fs::exists(m_top_dir.path()));
  fs::path test_tmpdir = m_top_dir.path() / "tmpdir";
  fs::create_directory(test_tmpdir);
  setEnv("TMPDIR", test_tmpdir.c_str(), 1);
  string tmp_env_val = getEnv("TMPDIR");
  // test that the variable is actually set in the environment
  // of the process
  BOOST_CHECK(tmp_env_val == test_tmpdir.string());

  // create a new temporary directory that should be rooted at the
  // value of the TMPDIR directory.

  TempDir new_one;

  // test that the new tmp directory has been created in the right
  // directory (in $TMPDIR)
  BOOST_CHECK(new_one.path().parent_path() == test_tmpdir);

  // remove the environment variable
  unSetEnv("TMPDIR");
  // check that it is gone
  BOOST_CHECK(getEnv("TMPDIR") == "");
  BOOST_CHECK(fs::exists(test_tmpdir));
}

BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------
