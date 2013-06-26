/*
 * Time_test.cpp
 *
 *  Created on: Jan 13, 2011
 *      Author: Marco Clemencic
 */

#include "ElementsKernel/Timing.h"
#include "ElementsKernel/Sleep.h"

// from SPI version of the testdriver
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/XmlOutputter.h>

// to produce one line per test
#include <cppunit/TestResult.h>
//#include <cppunit/BriefTestProgressListener.h>
//#include <cppunit/TestResultCollector.h>

// needed by the implementation of cool::ProgressListener
#include <cppunit/Test.h>
#include <cppunit/TestFailure.h>
#include <cppunit/TestListener.h>

// provides macros for the tests
#include <cppunit/extensions/HelperMacros.h>

namespace ElementsKernelTest {
class TimingTest: public CppUnit::TestFixture {

CPPUNIT_TEST_SUITE( TimingTest );

  CPPUNIT_TEST(test_adjustTime);
  CPPUNIT_TEST(test_ProcessTime);

  CPPUNIT_TEST_SUITE_END()
  ;

public:

  TimingTest() {
  }
  virtual ~TimingTest() {
  }

  void test_adjustTime() {
    int64_t t = 12345678901234560LL; // units of 100 nanoseconds
    // 1234567890123456000 nanoseconds
    //    1234567890123456 microseconds
    //       1234567890123 milliseconds
    //          1234567890 seconds
    //            20576131 minutes
    //              342935 hours
    //               14288 days
    //                 476 months (of 30 days)
    //                  39 years (of 365 days)
    //   12345678901234560 native
    CPPUNIT_ASSERT_EQUAL(
        Elements::System::adjustTime<Elements::System::nanoSec>(t),
        (int64_t)1234567890123456000LL);
    CPPUNIT_ASSERT_EQUAL(
        Elements::System::adjustTime<Elements::System::microSec>(t),
        (int64_t)1234567890123456LL);
    CPPUNIT_ASSERT_EQUAL(
        Elements::System::adjustTime<Elements::System::milliSec>(t),
        (int64_t)1234567890123LL);
    CPPUNIT_ASSERT_EQUAL(Elements::System::adjustTime<Elements::System::Sec>(t),
    	(int64_t)1234567890LL);
    CPPUNIT_ASSERT_EQUAL(Elements::System::adjustTime<Elements::System::Min>(t),
    	(int64_t)20576131LL);
    CPPUNIT_ASSERT_EQUAL(
        Elements::System::adjustTime<Elements::System::Hour>(t), (int64_t)342935LL);
    CPPUNIT_ASSERT_EQUAL(Elements::System::adjustTime<Elements::System::Day>(t),
    	(int64_t)14288LL);
    CPPUNIT_ASSERT_EQUAL(
        Elements::System::adjustTime<Elements::System::Month>(t), (int64_t)476LL);
    CPPUNIT_ASSERT_EQUAL(
        Elements::System::adjustTime<Elements::System::Year>(t), (int64_t)39LL);
    CPPUNIT_ASSERT_EQUAL(
        Elements::System::adjustTime<Elements::System::Native>(t),
        (int64_t)12345678901234560LL);

  }

  void test_ProcessTime() {
    Elements::System::ProcessTime t0 = Elements::System::getProcessTime();
    Elements::normalSleep(1);
    Elements::System::ProcessTime t1 = Elements::System::getProcessTime();

    // light check on the conversion
    CPPUNIT_ASSERT_EQUAL(t1.kernelTime<Elements::System::Sec>(),
        t1.kernelTime<Elements::System::milliSec>() / 1000);

    // measure the elapsed time
    CPPUNIT_ASSERT_EQUAL((t1 - t0).elapsedTime<Elements::System::Sec>(), (int64_t)1LL);
  }

  void setUp() {
  }

  void tearDown() {
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(TimingTest);

}
