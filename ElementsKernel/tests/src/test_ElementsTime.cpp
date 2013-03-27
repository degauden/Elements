/*
 * Time_test.cpp
 *
 *  Created on: Jan 13, 2011
 *      Author: Marco Clemencic
 */

#include "ElementsKernel/Time.h"

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
//#include <cppunit/Test.h>
#include <cppunit/TestFailure.h>
#include <cppunit/TestListener.h>

// provides macros for the tests
#include <cppunit/extensions/HelperMacros.h>

namespace ElementsKernelTest {
class TimeTest: public CppUnit::TestFixture {

CPPUNIT_TEST_SUITE( TimeTest );

  CPPUNIT_TEST(test_nanoformat);
  CPPUNIT_TEST(test_format);

  CPPUNIT_TEST_SUITE_END()
  ;

public:

  TimeTest() {
  }
  virtual ~TimeTest() {
  }

  void test_nanoformat() {
    Elements::Time t(2011, 0, 13, 14, 22, 45, 123000, true);

    CPPUNIT_ASSERT_EQUAL(std::string("000123"), t.nanoformat());
    CPPUNIT_ASSERT_EQUAL(std::string("000123000"), t.nanoformat(9));
    CPPUNIT_ASSERT_EQUAL(std::string("000123"), t.nanoformat(2, 7));
    CPPUNIT_ASSERT_EQUAL(std::string("0"), t.nanoformat(1, 3));
    CPPUNIT_ASSERT_EQUAL(std::string("00012"), t.nanoformat(1, 5));
    CPPUNIT_ASSERT_EQUAL(std::string("000"), t.nanoformat(3, 3));

  }

  void test_format() {
    Elements::Time t(2011, 0, 13, 14, 22, 45, 1230000, true);

    CPPUNIT_ASSERT_EQUAL(std::string("2011-01-13 14:22:45"),
        t.format(true, "%Y-%m-%d %H:%M:%S"));
    CPPUNIT_ASSERT_EQUAL(std::string("2011-01-13 14:22:45.001"),
        t.format(true, "%Y-%m-%d %H:%M:%S.%f"));
    CPPUNIT_ASSERT_EQUAL(std::string("14:22:45 2011/01/13 "),
        t.format(true, "%H:%M:%S %Y/%m/%d "));

  }

  void setUp() {
  }

  void tearDown() {
  }

};

CPPUNIT_TEST_SUITE_REGISTRATION(TimeTest);

}
