/**
 * @file ThisModule_test.cpp
 *
 * @date Dec 10, 2014
 * @author Hubert Degaudenzi
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

#include "ElementsKernel/ThisModule.h"

#include <boost/test/unit_test.hpp>

#include <libgen.h>

namespace Elements {

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE(ThisModule_test)
//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ThisModuleName_test) {

  // Get the present module. Here this must be the test executable
  BOOST_CHECK_EQUAL(System::getThisModuleInfo().name(), "ThisModule_test");
}

BOOST_AUTO_TEST_CASE(ThisExeName_test) {

  // Get the present module. Here this must be the test executable

  BOOST_CHECK_EQUAL(::basename(const_cast<char*>(System::exeName().c_str())), "ThisModule_test");
}

BOOST_AUTO_TEST_CASE(ThisModuleConversion_test) {

  Dl_info info = System::getThisModuleInfo();

  BOOST_CHECK_EQUAL(::basename(const_cast<char*>(info.dli_fname)), "ThisModule_test");
}

//-----------------------------------------------------------------------------
BOOST_AUTO_TEST_SUITE_END()
//-----------------------------------------------------------------------------
//
// End of the Boost tests

}  // namespace Elements
