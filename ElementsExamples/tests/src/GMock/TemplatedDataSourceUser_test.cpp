/*
 * @file TemplatedDataSourceUser_test.cpp
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

#include "ElementsExamples/TemplatedDataSourceUser.h"  // Access the objects you want to test

#include "ElementsKernel/EnableGMock.h"
#include "ElementsKernel/Real.h"                       // isEqual

#include "DataSourceUserTemplatedTypeMock.h"           // Access the needed mock objects.

using testing::Return;
using Elements::Examples::DataSourceUserTemplatedTypeMock;

BOOST_AUTO_TEST_SUITE(TemplatedDataSourceUser_test_suite)

BOOST_AUTO_TEST_CASE(sumRecords_test) {
  // Setup mock
  DataSourceUserTemplatedTypeMock data_source_mock;
  EXPECT_CALL(data_source_mock, countRecords()).Times(1).WillOnce(testing::Return(5));
  for (size_t index = 0; index < 5; ++index) {
    EXPECT_CALL(data_source_mock, getRecordValue(index)).Times(1).WillOnce(Return(index+1.));
  }

  // object to test
  Elements::Examples::TemplatedDataSourceUser user{};
  double result = user.sumRecords<DataSourceUserTemplatedTypeMock>(data_source_mock);

  BOOST_CHECK_MESSAGE(Elements::isEqual(result, 15.), "Expected value :" << 15 << " Actual value :" << result);
}

// Ends the test suite
BOOST_AUTO_TEST_SUITE_END()

