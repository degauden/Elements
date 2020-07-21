/**
 * @file MathConstants_test.cpp
 *
 * @date Dec 10, 2014
 * @author hubert
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
 */

#include "ElementsKernel/SystemOfUnits.h"  // The interface to test

#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test_suite.hpp>
#include <boost/math/constants/constants.hpp>

#include "ElementsKernel/Real.h"   // for the isEqual function

namespace Elements {

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(SystemOfUnits_test)
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(LuminousFlux_test) {

  using Units::perMillion;
  using Units::jansky;
  using Units::microjansky;

  BOOST_CHECK(isEqual(microjansky, jansky*perMillion));
}

BOOST_AUTO_TEST_CASE(Illuminance_test) {

  using Units::lux;
  using Units::lumen;
  using Units::meter2;

  BOOST_CHECK(isEqual(lux, lumen/meter2));
}

BOOST_AUTO_TEST_CASE(Misc_test) {

  using Units::perCent;
  BOOST_CHECK(isEqual(perCent, 1.0/100.0));

  using Units::perThousand;
  BOOST_CHECK(isEqual(perThousand, 1.0/1000.0));

  using Units::perMillion;
  BOOST_CHECK(isEqual(perMillion, 1.0/1.0e6));
}


//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE_END()
//-----------------------------------------------------------------------------
//
// End of the Boost tests

}  // namespace Elements
