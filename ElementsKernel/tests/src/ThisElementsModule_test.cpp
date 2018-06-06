/**
 * @file ThisProject_test.cpp
 *
 * @date May 30, 2016
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

#include "ThisElementsModule.h"

#include <typeinfo>                  // for type_info
#include <string>

#include <boost/test/unit_test.hpp>

using std::string;

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(ThisElementsModule_test)
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ThisProjectName_test) {

  // Get the present module. Here this must be the test executable
  BOOST_CHECK_EQUAL(THIS_MODULE_NAME_STRING, "ElementsKernel");

}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE_END()
//-----------------------------------------------------------------------------
//
// End of the Boost tests
