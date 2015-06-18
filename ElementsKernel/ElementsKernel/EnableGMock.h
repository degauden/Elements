/**
 * @file EnableGMock.h
 * @date December 16, 2014
 * @author Nikolaos Apostolakos
 */

#ifndef ENABLEGMOCK_H
#define ENABLEGMOCK_H

#include <gmock/gmock.h>
#include <boost/test/unit_test.hpp>

#include "ElementsKernel/Export.h" // ELEMENTS_API

class BoostTestAdapter : public testing::EmptyTestEventListener {

  virtual void OnTestPartResult(const testing::TestPartResult& testPartResult) {
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

BOOST_GLOBAL_FIXTURE(GoogleMockSetupFixture)

#endif	/* ENABLEGMOCK_H */

