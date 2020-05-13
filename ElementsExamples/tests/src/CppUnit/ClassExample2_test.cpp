/**
 * @file ClassExample_test.cpp
 *
 * Created on: Aug 12, 2013 Jun 20, 2013
 *     Author: Pierre Dubath
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

#include <iostream>
#include <string>                                // for string

#include <cppunit/TestFixture.h>
#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ElementsExamples/ClassExample2.h"

#include "ElementsKernel/Exception.h"
#include "ElementsKernel/Real.h"                 // Provides isEqual



using std::string;
using Elements::Examples::ClassExample2;

/*
 * Fixture to compare the test result against reference values
 */
class ClassExampleSuite : public CppUnit::TestFixture {

  CPPUNIT_TEST_SUITE(ClassExampleSuite);

  CPPUNIT_TEST(fundamentalTypeMethodTest);
  CPPUNIT_TEST(fundamentalTypeMethodSecondTest);
  CPPUNIT_TEST(gettersTest);
  CPPUNIT_TEST(exceptionInDivideNumbersTest);

  CPPUNIT_TEST_SUITE_END();

public:

  void setUp() override;
  void tearDown() override;
  ClassExampleSuite() = default;

protected:

  void fundamentalTypeMethodTest();
  void fundamentalTypeMethodSecondTest();
  void gettersTest();
  void exceptionInDivideNumbersTest();

private:

  // Elements::ClassExample m_class_example {};

  // expected static string (hard coded in .cpp file!)
  string m_expected_static_string {"This is a static field example"};

  // Some numbers to feed the constructor
  const int m_source_id {123456789};
  const double m_ra {266.40506655};
  const double m_input_variable {1.273645899};

// Number to test the methods
  const double m_expected_result {1.273645899};

  ClassExample2 m_class_example = ClassExample2::factoryMethod(m_source_id, m_ra);

};

//-----------------------------------------------------------------------------

void ClassExampleSuite::setUp() {
}

void ClassExampleSuite::tearDown() {
}

//-----------------------------------------------------------------------------



void ClassExampleSuite::gettersTest() {

  CPPUNIT_ASSERT(m_source_id == m_class_example.getSourceId());

}

void ClassExampleSuite::fundamentalTypeMethodTest() {

  CPPUNIT_ASSERT_DOUBLES_EQUAL(m_expected_result, m_class_example.fundamentalTypeMethod(m_input_variable),
                               Elements::DBL_DEFAULT_TEST_TOLERANCE);

}

void ClassExampleSuite::fundamentalTypeMethodSecondTest() {

  CPPUNIT_ASSERT(Elements::isEqual(m_expected_result, m_class_example.fundamentalTypeMethod(m_input_variable)));

}


void ClassExampleSuite::exceptionInDivideNumbersTest() {

  bool exception = false;
  try {
    m_class_example.divideNumbers(1.0, 0.0);
  } catch (const Elements::Exception & e) {
    // exception = true;
    string exception_str = e.what();
    exception = (exception_str.find("exception in ClassExample::divideNumbers") != string::npos);
  }
  CPPUNIT_ASSERT(exception);

}


//-----------------------------------------------------------------------------

CPPUNIT_TEST_SUITE_REGISTRATION(ClassExampleSuite);
