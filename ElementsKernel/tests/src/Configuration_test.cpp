/**
 * @file Configuration_test.cpp
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

#include "ElementsKernel/Configuration.h"          // header to test

#include <string>                         // for std::string
#include <vector>                         // for std::vector
#include <algorithm>                      // for for_each, transform, copy_if

#include <boost/test/unit_test.hpp>       // for boost unit test macros
#include <boost/filesystem.hpp>           // for boost::filesystem
#include <boost/filesystem/fstream.hpp>   // for ofstream

#include "ElementsKernel/Temporary.h"     // for TempDir, TempEnv
#include <ElementsKernel/Exception.h>     // for Exception
#include "ElementsKernel/Path.h"          // for joinPath
#include "ElementsKernel/System.h"        // for DEFAULT_INSTALL_PREFIX

using std::string;
using std::vector;

using boost::filesystem::path;
using boost::filesystem::exists;
using boost::filesystem::is_regular;

using Elements::TempDir;
using Elements::TempEnv;
using Elements::Path::join;
using Elements::System::DEFAULT_INSTALL_PREFIX;

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

struct Configuration_Fixture {

  TempDir m_top_dir;
  vector<path> m_item_list;
  vector<path> m_target_item_list;
  vector<path> m_real_item_list;
  vector<path> m_target_real_item_list;

  Configuration_Fixture(): m_top_dir{ "Configuration_test-%%%%%%%" } {

    using std::for_each;
    using std::copy_if;
    using std::distance;

    m_item_list.push_back(m_top_dir.path() / "test1");
    m_item_list.push_back(m_top_dir.path() / "test1" / "foo");
    m_item_list.push_back(m_top_dir.path() / "test2");
    m_item_list.push_back(m_top_dir.path() / "test3");

    for_each(m_item_list.cbegin(), m_item_list.cend(),
        [](path p) {
        boost::filesystem::create_directory(p);
    });

    m_item_list.push_back(m_top_dir.path() / "test4");

    m_target_item_list = m_item_list;

    m_target_item_list.push_back(path(DEFAULT_INSTALL_PREFIX) / "share" / "conf");

    m_real_item_list.resize(m_item_list.size());
    auto it = copy_if(m_item_list.begin(), m_item_list.end(),
                      m_real_item_list.begin(),
                      [](const path& p){
                            return exists(p);
                      });
    m_real_item_list.resize(distance(m_real_item_list.begin(), it));

    m_target_real_item_list.resize(m_target_item_list.size());
    auto it2 = copy_if(m_target_item_list.begin(), m_target_item_list.end(),
                      m_target_real_item_list.begin(),
                      [](const path& p){
                            return exists(p);
                      });
    m_target_real_item_list.resize(distance(m_target_real_item_list.begin(), it2));


  }

  ~Configuration_Fixture() {

  }

};

BOOST_AUTO_TEST_SUITE(Configuration_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ConfigurationException_test) {

  BOOST_CHECK_THROW(Elements::getConfigurationPath("NonExistingFile.conf"), Elements::Exception);

}

BOOST_AUTO_TEST_CASE(ConfigurationVariableName_test) {

  BOOST_CHECK_EQUAL(Elements::getConfigurationVariableName(), "ELEMENTS_CONF_PATH");

}

BOOST_FIXTURE_TEST_CASE(getFromLocations_test, Configuration_Fixture) {

  auto env = TempEnv();

  env["ELEMENTS_CONF_PATH"] = join(m_item_list);

  auto locations = Elements::getConfigurationLocations();

  BOOST_CHECK_EQUAL_COLLECTIONS(locations.begin(), locations.end(),
                                m_target_item_list.begin(), m_target_item_list.end());

}

BOOST_FIXTURE_TEST_CASE(getFromLocationsExist_test, Configuration_Fixture) {

  auto env = TempEnv();

  env["ELEMENTS_CONF_PATH"] = join(m_real_item_list);

  auto locations = Elements::getConfigurationLocations(true);

  BOOST_CHECK_EQUAL_COLLECTIONS(locations.begin(), locations.end(),
                                m_target_real_item_list.begin(), m_target_real_item_list.end());

}


BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------
