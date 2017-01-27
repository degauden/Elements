/*
 * Copyright (C) 2012-2020 Euclid Science Ground Segment    
 *  
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free 
 * Software Foundation; either version 3.0 of the License, or (at your option)  
 * any later version.  
 *  
 * This library is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more  
 * details.  
 *  
 * You should have received a copy of the GNU Lesser General Public License 
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA  
 */

/* 
 * @file tests/src/Exception_test.cpp
 * @author nikoapos
 */

#include <string>
#include <boost/test/unit_test.hpp>       // for boost unit test macros

#include "ElementsKernel/Exception.h"

using std::string;

struct TestException : public Elements::Exception {
    
  explicit TestException(const string& message="") : Exception(message, Elements::ExitCode::NOT_OK) {
    m_number = -1;
  }
  
  explicit TestException(const int number) : Exception("", Elements::ExitCode::NOT_OK) {
    m_number = number;
  }
  
  int m_number;
};

BOOST_AUTO_TEST_SUITE(Exception_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(SubclassStreamOperator_test) {
  
  // Given
  string message_part_1 = "Part 1";
  string message_part_2 = "Part 2";
  
  // When
  try {
    throw TestException(message_part_1);
    
  // Then
  } catch (const TestException& ex) {
    BOOST_CHECK_EQUAL(ex.what(), message_part_1);
    BOOST_CHECK_EQUAL(ex.m_number, -1);
  } catch (const Elements::Exception& ex) {
    BOOST_FAIL("Exception subclass thrown as Elements::Exception");
  } catch (...) {
    BOOST_FAIL("Unknown type of exception instead of TestException");
  }
  
  // When
  try {
    throw TestException(5) << message_part_1 << message_part_2;
    
  // Then
  } catch (const TestException& ex) {
    BOOST_CHECK_EQUAL(ex.what(), message_part_1 + message_part_2);
    BOOST_CHECK_EQUAL(ex.m_number, 5);
  } catch (const Elements::Exception& ex) {
    BOOST_FAIL("Exception subclass thrown as Elements::Exception");
  } catch (...) {
    BOOST_FAIL("Unknown type of exception instead of TestException");
  }
  
  // When
  try {
    TestException e {6};
    throw e << message_part_1 << message_part_2;
    
  // Then
  } catch (const TestException& ex) {
    BOOST_CHECK_EQUAL(ex.what(), message_part_1 + message_part_2);
    BOOST_CHECK_EQUAL(ex.m_number, 6);
  } catch (const Elements::Exception& ex) {
    BOOST_FAIL("Exception subclass thrown as Elements::Exception");
  } catch (...) {
    BOOST_FAIL("Unknown type of exception instead of TestException");
  }
  
  // When
  try {
    TestException e {7};
    e << message_part_1 << message_part_2;
    throw e;
    
  // Then
  } catch (const TestException& ex) {
    BOOST_CHECK_EQUAL(ex.what(), message_part_1 + message_part_2);
    BOOST_CHECK_EQUAL(ex.m_number, 7);
  } catch (const Elements::Exception& ex) {
    BOOST_FAIL("Exception subclass thrown as Elements::Exception");
  } catch (...) {
    BOOST_FAIL("Unknown type of exception instead of TestException");
  }
  
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()
