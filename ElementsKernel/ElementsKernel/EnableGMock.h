/**
 * @file EnableGMock.h
 * @brief Google Mock helper classes
 * @date December 16, 2014
 * @author Nikolaos Apostolakos
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

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_ENABLEGMOCK_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_ENABLEGMOCK_H_

#include <gmock/gmock.h>
#include <boost/test/unit_test.hpp>
#include <boost/version.hpp>            // for the BOOST_VERSION define

#include "ElementsKernel/Export.h"      // ELEMENTS_API

class BoostTestAdapter: public testing::EmptyTestEventListener {

  void OnTestPartResult(const testing::TestPartResult& testPartResult) override {
    if (testPartResult.failed()) {
      BOOST_ERROR(testPartResult.summary());
    }
  }

};

class GoogleMockSetupFixture {

public:

  GoogleMockSetupFixture() {
    testing::InitGoogleMock(
        &boost::unit_test::framework::master_test_suite().argc,
        boost::unit_test::framework::master_test_suite().argv);
    auto& listeners = testing::UnitTest::GetInstance()->listeners();
    delete listeners.Release(listeners.default_result_printer());
    listeners.Append(new BoostTestAdapter);
  }

};

#if BOOST_VERSION >= 105900
BOOST_GLOBAL_FIXTURE(GoogleMockSetupFixture);
#else
BOOST_GLOBAL_FIXTURE(GoogleMockSetupFixture)
#endif

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_ENABLEGMOCK_H_

/**@}*/
