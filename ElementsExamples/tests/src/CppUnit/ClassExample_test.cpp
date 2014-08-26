/**
 * @file ClassExample_test.cpp
 *
 * Created on: Aug 12, 2013 Jun 20, 2013
 *     Author: Pierre Dubath
 */

#include <iostream>
#include <cppunit/TestFixture.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>

#include "ElementsKernel/Exception.h"
#include "ElementsKernel/Real.h" // Provides isEqual

#include "ElementsExamples/ClassExample.h"

#include "tests/src/Tolerance.h"

using namespace std;

/*
 * Fixture to compare the test result against reference values
 */
class ClassExampleSuite : public CppUnit::TestFixture {

  CPPUNIT_TEST_SUITE(ClassExampleSuite);

  CPPUNIT_TEST(constructorsTest);
  CPPUNIT_TEST(gettersTest);
  CPPUNIT_TEST(computeSumTest);
  CPPUNIT_TEST(divideNumbersTest);
  CPPUNIT_TEST(divideNumbersByZeroExceptionTest);
  CPPUNIT_TEST(summingAndDividingTest);
  CPPUNIT_TEST(summingAndDividingByZeroExceptionTest);

  CPPUNIT_TEST_SUITE_END();

public:

  void setUp();
  void tearDown();
  ClassExampleSuite() = default;

protected:

  void constructorsTest();
  void gettersTest();
  void computeSumTest();
  void divideNumbersTest();
  void divideNumbersByZeroExceptionTest();
  void summingAndDividingTest();
  void summingAndDividingByZeroExceptionTest();



private:

  ClassExample* m_class_example_ptr { nullptr };

  // Some numbers to feed the constructor
  const int64_t m_source_id { 16253 };
  const double m_ra  { 64.5768 };
  const double m_dec { -34.2857 };

  // expected static string (hard coded in .cpp file!)
  std::string m_expected_static_string {"This is a static field example"};

  // Numbrs to test the methods
  const double m_first_number { 2.5647 };
  const double m_second_number { 5.6874 };

  const double m_expected_sum { 8.2521 };
  const double m_expected_division_result { 0.45094419242536132 };
  const double m_expected_final_result { m_expected_division_result } ;


};

//-----------------------------------------------------------------------------


void ClassExampleSuite::setUp() {
  m_class_example_ptr = new ClassExample(m_source_id, m_ra, m_dec);
}

void ClassExampleSuite::tearDown() {
  delete m_class_example_ptr;
}

//-----------------------------------------------------------------------------


void ClassExampleSuite::constructorsTest() {

  CPPUNIT_ASSERT(m_class_example_ptr);

}

void ClassExampleSuite::gettersTest() {

  CPPUNIT_ASSERT(m_source_id == m_class_example_ptr->getSourceId());
  CPPUNIT_ASSERT_DOUBLES_EQUAL(m_ra, m_class_example_ptr->getRa(), TEST_DOUBLE_TOLERANCE);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(m_dec, m_class_example_ptr->getDec(), TEST_DOUBLE_TOLERANCE);
  CPPUNIT_ASSERT(isEqual(m_ra,m_class_example_ptr->getRa()));
  CPPUNIT_ASSERT(isEqual(m_dec, m_class_example_ptr->getDec()));
  CPPUNIT_ASSERT(m_expected_static_string == m_class_example_ptr->getStaticString());

}


void ClassExampleSuite::computeSumTest() {

  double actualSum = m_class_example_ptr->computeSum(m_first_number, m_second_number);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(actualSum, m_expected_sum, TEST_DOUBLE_TOLERANCE);

}

void ClassExampleSuite::divideNumbersTest() {

  double actualDivisionResult = m_class_example_ptr->divideNumbers(m_first_number, m_second_number);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(actualDivisionResult, m_expected_division_result, TEST_DOUBLE_TOLERANCE);

}

void ClassExampleSuite::divideNumbersByZeroExceptionTest() {

  bool exception = false;
  try {
    m_class_example_ptr->divideNumbers(m_first_number, 0.0);
  } catch (const Elements::Exception & e) {
    //exception = true;
    string exception_str = e.what();
    exception =
        (exception_str.find(
            "exception in divideNumbers")
            != string::npos);
  }
  CPPUNIT_ASSERT(exception);

}

void ClassExampleSuite::summingAndDividingTest() {

  m_class_example_ptr->summingAndDividing(m_first_number, m_second_number);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(m_class_example_ptr->getResult(), m_expected_final_result, TEST_DOUBLE_TOLERANCE);

}

void ClassExampleSuite::summingAndDividingByZeroExceptionTest() {

  bool exception = false;
  try {
    m_class_example_ptr->summingAndDividing(m_first_number, 0.0);
  } catch (const Elements::Exception & e) {
    //exception = true;
    string exception_str = e.what();
    exception =
        (exception_str.find(
            "exception in divideNumbers")
            != string::npos);
  }
  CPPUNIT_ASSERT(exception);

}

//-----------------------------------------------------------------------------

CPPUNIT_TEST_SUITE_REGISTRATION(ClassExampleSuite);
