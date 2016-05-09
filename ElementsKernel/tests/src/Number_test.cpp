/**
 * @file Number_test.cpp
 *
 * @date May 22, 2013
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
#include "ElementsKernel/Number.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(Number_test)

BOOST_AUTO_TEST_CASE(NumberCast_test) {

  using Elements::numberCast;

  BOOST_CHECK_EQUAL(numberCast<int>(3.6), 4);
  BOOST_CHECK_EQUAL(static_cast<int>(3.6), 3);
  BOOST_CHECK_EQUAL(numberCast<int>(3.2), 3);
  BOOST_CHECK_NE(numberCast<int>(3.6), int(3.6));
  BOOST_CHECK_EQUAL(numberCast<int>(3.2), int(3.2));


}

//-----------------------------------------------------------------------------
// End of the Boost tests
BOOST_AUTO_TEST_SUITE_END()
