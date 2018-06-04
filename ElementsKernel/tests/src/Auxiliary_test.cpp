/**
 * @file Auxiliary_test.cpp
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

#include "ElementsKernel/Auxiliary.h"          // header to test

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

struct Auxiliary_Fixture {

  TempDir m_top_dir;
  vector<path> m_item_list;

  Auxiliary_Fixture(): m_top_dir{ "Auxiliary_test-%%%%%%%" } {

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

  ~Auxiliary_Fixture() {

  }

};

BOOST_AUTO_TEST_SUITE(Auxiliary_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(AuxiliaryConstructor_test) {

  path make_template = Elements::getAuxiliaryPath("ElementsKernel/templates/Makefile.in");

}

BOOST_FIXTURE_TEST_CASE(getFromLocations_test, Auxiliary_Fixture) {


}

BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------
