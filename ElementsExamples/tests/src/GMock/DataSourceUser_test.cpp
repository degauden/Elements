/*
 * DataSourceUser_test.cpp
 *
 *  Created on: Sep 23, 2015
 *      Author: fdubath
 */

#include "ElementsKernel/Real.h" // isEqual

#include "ElementsExamples/DataSourceUser.h" // Access the objects you want to test
#include "DataSourceInterfaceMock.h" // Access the needed mock objects.


using namespace Elements;
using namespace ElementsExamples;


BOOST_AUTO_TEST_SUITE (DataSourceUser_test_suite)

BOOST_AUTO_TEST_CASE(sumRecords_test) {

  // Setup mock
  DataSourceInterfaceMock data_source_mock;

  EXPECT_CALL(data_source_mock, countRecords()).Times(1).WillOnce(Return(5));

  for (size_t index=0; index<5;++index){
    EXPECT_CALL(data_source_mock, getRecordValue(index)).Times(1).WillOnce(Return(index+1.));
  }

  // object to test
  DataSourceUser user{};
  double result = user.sumRecords(data_source_mock);

  BOOST_CHECK(Elements::isEqual(result,15.));
}

// Ends the test suite
BOOST_AUTO_TEST_SUITE_END ()

