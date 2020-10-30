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

#include "ElementsKernel/ModuleInfo.h"  // header file to test

#include <libgen.h>  // for basename

#include <iostream>
#include <string>

#include <boost/test/unit_test.hpp>  // for the boost test macros

#include "ElementsKernel/ThisModule.h"  // for getThisModuleInfo

using std::string;

namespace Elements {

BOOST_AUTO_TEST_SUITE(ModuleInfo_test)

//-----------------------------------------------------------------------------
struct ModuleInfo_Fixture {

  ModuleInfo_Fixture() {}

  ~ModuleInfo_Fixture() {}
};

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(GetExecutablePath_test) {

  auto exe_path = System::getExecutablePath();

  BOOST_CHECK(exe_path.filename().string() == "ModuleInfo_test");
}

BOOST_AUTO_TEST_CASE(ExeName_test) {

  auto   exe_path = System::getExecutablePath();
  string name     = System::exeName();

  BOOST_CHECK_EQUAL(exe_path.string(), name);
}

BOOST_AUTO_TEST_CASE(SelfProc_test) {

  auto proc_path = System::getSelfProc();

  BOOST_CHECK(not proc_path.empty());
}

BOOST_AUTO_TEST_CASE(libraryName_test) {
  const System::ModuleInfo& info = System::getThisModuleInfo();

  BOOST_CHECK_EQUAL(::basename(const_cast<char*>(info.libraryName().c_str())), "ModuleInfo_test");
}

BOOST_AUTO_TEST_CASE(addresse_test) {

  const System::ModuleInfo& info = System::getThisModuleInfo();

  BOOST_CHECK_EQUAL(info.addresse(), static_cast<void*>(0));
}

BOOST_AUTO_TEST_CASE(moduleName_test) {

  auto module_name = System::moduleName();

  BOOST_CHECK_EQUAL(module_name, "libElementsKernel");
}

BOOST_AUTO_TEST_CASE(moduleNameFull_test) {

  auto module_name_full = System::moduleNameFull();

  string module = ::basename(const_cast<char*>(module_name_full.c_str()));

  BOOST_CHECK_EQUAL(module.substr(static_cast<string::size_type>(0), module.find('.')), "libElementsKernel");
}

BOOST_AUTO_TEST_CASE(exeHandle_test) {

  auto exe_handle = System::exeHandle();

  BOOST_CHECK_NE(exe_handle, static_cast<void*>(0));
}

BOOST_AUTO_TEST_CASE(linkedModules_test) {

  auto linked_modules = System::linkedModules();

  BOOST_CHECK(linked_modules.size() > 0);
}

BOOST_AUTO_TEST_CASE(linkedModulePaths_test) {

  auto linked_module_path = System::linkedModulePaths();

  BOOST_CHECK(linked_module_path.size() > 0);
}

BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------

}  // namespace Elements
