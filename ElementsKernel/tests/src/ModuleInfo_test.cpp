/**
 * @file ModuleInfo_test.cpp
 *
 * @date Jul 26, 2016
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


#include <iostream>
#include <string>

#include <boost/test/unit_test.hpp>          // for the boost test macros
#include <boost/filesystem.hpp>              // for boost/filesystem

#include "ElementsKernel/ModuleInfo.h"       // header file to test

BOOST_AUTO_TEST_SUITE(ModuleInfo_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(GetExecutablePath_test) {

  boost::filesystem::path exe_path = Elements::System::getExecutablePath();

  BOOST_CHECK(exe_path.filename().string() == "ModuleInfo_test");

}

BOOST_AUTO_TEST_CASE(ExeName_test) {

  boost::filesystem::path exe_path = Elements::System::getExecutablePath();
  std::string name = Elements::System::exeName();

  BOOST_CHECK_EQUAL(exe_path.string(), name);

}


BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------





