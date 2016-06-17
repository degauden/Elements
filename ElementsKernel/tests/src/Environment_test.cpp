/**
 * @file Environment_test.cpp
 *
 * @date Jun 17, 2016
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

#include <boost/test/unit_test.hpp>

#include "ElementsKernel/Environment.h"        // for Environment

using Elements::Environment;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

struct Environment_fixture {

  Environment base;

  Environment_fixture() {
    // setup
  }
  ~Environment_fixture() {
    // teardown
  }
};

BOOST_AUTO_TEST_SUITE(Environment_test)

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(Base_test, Environment_fixture) {


}


BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------
