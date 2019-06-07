/*
 * DataSourceUser_test.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: fdubath
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

#include "ElementsExamples/DataSourceUser.h"  // Access the objects you want to test

#include <cstddef>
#include <gmock/gmock.h>
#include <boost/test/unit_test.hpp>

#include "ElementsKernel/Real.h"
#include "ElementsKernel/EnableGMock.h"       // initialize the gmock framework

#include "DataSourceInterfaceMock.h"

using std::size_t;
using testing::Return;

BOOST_AUTO_TEST_SUITE(DataSourceUser_test_suite)

BOOST_AUTO_TEST_CASE(sumRecords_test) {

  // Setup mock
  Elements::Examples::DataSourceInterfaceMock data_source_mock;

  EXPECT_CALL(data_source_mock, countRecords()).Times(1).WillOnce(Return(5));

  for (size_t index = 0; index < 5; ++index) {
    EXPECT_CALL(data_source_mock, getRecordValue(index)).Times(1).WillOnce(Return(index+1.));
  }

  // object to test
  Elements::Examples::DataSourceUser user{};
  double result = user.sumRecords(data_source_mock);

  BOOST_CHECK_MESSAGE(Elements::isEqual(result, 15.), "Expected value :"<< 15. <<" Actual value :" << result);
}

// Ends the test suite
BOOST_AUTO_TEST_SUITE_END()

