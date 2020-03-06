/**
 * @file BackTrace_test.cpp
 *
 * @date Aug 27, 2015
 * @author hubert
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

#include "ElementsKernel/System.h"

#include <sys/utsname.h>

#include <string>
#include <vector>
#include <boost/test/unit_test.hpp>

#include "ElementsKernel/Environment.h"

using std::string;

BOOST_AUTO_TEST_SUITE(System_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(HostName_test) {

  using std::string;

  Elements::Environment current;

  if (current["HOSTNAME"].exists() and string(current["HOSTNAME"]) != ".") {
    BOOST_CHECK_EQUAL(string(current["HOSTNAME"]), Elements::System::hostName());
  }

}

BOOST_AUTO_TEST_CASE(osName_test) {

  string osname = "UNKNOWN";
  struct utsname ut;
  if (::uname(&ut) == 0) {
    osname = ut.sysname;
  }

  BOOST_CHECK_EQUAL(Elements::System::osName(), osname);

}

BOOST_AUTO_TEST_CASE(osVersion_test) {


  string osver = "UNKNOWN";
  struct utsname ut;
  if (uname(&ut) == 0) {
    osver = ut.release;
  }

  BOOST_CHECK_EQUAL(Elements::System::osVersion(), osver);

}


//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

