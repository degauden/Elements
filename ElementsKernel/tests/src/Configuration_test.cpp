/**
 * @file Configuration_test.cpp
 *
 * Created on: Dec 4, 2013
 *     Author: Pierre Dubath
 */

#include "ElementsKernel/Configuration.h"          // header to test

#include <string>                         // for std::string
#include <vector>                         // for std::vector
#include <algorithm>                      // for for_each, transform

#include <boost/test/unit_test.hpp>       // for boost unit test macros
#include <boost/filesystem.hpp>           // for boost::filesystem
#include <boost/filesystem/fstream.hpp>   // for ofstream

#include <ElementsKernel/Temporary.h>     // for TempDir

using std::string;
using std::vector;

using boost::filesystem::path;
using boost::filesystem::exists;
using boost::filesystem::is_regular;

using Elements::TempDir;

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

struct Configuration_Fixture {

  TempDir m_top_dir;
  vector<path> m_item_list;

  Configuration_Fixture(): m_top_dir{ "Configuration_test-%%%%%%%" } {

    using std::for_each;

    m_item_list.push_back(m_top_dir.path() / "test1");
    m_item_list.push_back(m_top_dir.path() / "test1" / "foo");
    m_item_list.push_back(m_top_dir.path() / "test2");
    m_item_list.push_back(m_top_dir.path() / "test3");

    for_each(m_item_list.cbegin(), m_item_list.cend(),
        [](path p) {
        boost::filesystem::create_directory(p);
    });

  }

  ~Configuration_Fixture() {

  }

};

BOOST_AUTO_TEST_SUITE(Configuration_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ConfigurationConstructor_test) {


}

BOOST_FIXTURE_TEST_CASE(getFromLocations_test, Configuration_Fixture) {


}


BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------
