/**
 * @file Slepp_test.cpp
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
#include "ElementsKernel/Sleep.h"

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(Sleep_test)

BOOST_AUTO_TEST_CASE(SleepCall_test) {

  Elements::normalSleep(1);
  Elements::nanoSleep(10);

}

//-----------------------------------------------------------------------------
// End of the Boost tests
BOOST_AUTO_TEST_SUITE_END()
