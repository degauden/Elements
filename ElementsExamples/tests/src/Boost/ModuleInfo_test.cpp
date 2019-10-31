/**
 * @file ClassExample_test.cpp
 *
 * Created on: Aug 12, 2013Jun 20, 2013
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

#include "ElementsExamples/ModuleInfo.h"

#include <string>                     // for std::string

#include <boost/test/unit_test.hpp>

#include "ThisProject.h"

//-----------------------------------------------------------------------------

/*
 * Fixture to compare the test result against reference values
 */
struct ModuleInfoFixture {

  ModuleInfoFixture() {
    // call the constructor
  }


  ~ModuleInfoFixture() {
    // delete fixture object
  }


};

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(ModuleInfo_test)

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(ModuleName_test, ModuleInfoFixture ) {
  if (THIS_PROJECT_USE_SOVERSION) {
    BOOST_CHECK_EQUAL(Elements::Examples::getModuleInfo().name(),
                      std::string(Elements::System::LIB_PREFIX
                      + "ElementsExamples"
                      + Elements::System::LIB_SUFFIX
                      + "." + THIS_PROJECT_ORIGINAL_VERSION));
  } else {
    BOOST_CHECK_EQUAL(Elements::Examples::getModuleInfo().name(),
                      std::string(Elements::System::LIB_PREFIX
                      + "ElementsExamples"
                      + Elements::System::LIB_SUFFIX));

  }
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()
