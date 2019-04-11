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

#include <string>
#include <vector>

#define BOOST_TEST_IGNORE_NON_ZERO_CHILD_CODE
#include <boost/test/unit_test.hpp>

#include "ElementsServices/DataSync/DataSyncUtils.h"

namespace DataSync = ElementsServices::DataSync;

using std::string;
using std::vector;

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(DataSyncUtils_test)

//-----------------------------------------------------------------------------

void checkLower(vector<string> casedList, string uncased) {
  for (const auto& cased : casedList) {
    BOOST_CHECK_EQUAL(DataSync::lower(cased), uncased);
  }
}

BOOST_AUTO_TEST_CASE(lower_test) {
  const vector<string> casedList { "webdav", "WebDAV", "WEBDAV" };
  checkLower(casedList, "webdav");
}

BOOST_AUTO_TEST_CASE(checkCall_test) {
  BOOST_CHECK(DataSync::checkCall("ls"));
  BOOST_CHECK(not DataSync::checkCall("This command should not work"));
}

BOOST_AUTO_TEST_CASE(runCommand_out_test) {
  const string msg("toto");
  const auto outerr = DataSync::runCommandAndCaptureOutErr("echo " + msg);
  auto output = outerr.first;
  const string::size_type size = output.size();
  if (output[size - 1] == '\n') {
    output.resize(size - 1);
  }
  BOOST_CHECK_EQUAL(output, msg);
}

BOOST_AUTO_TEST_CASE(localWorkspacePrefix_test) {
  const string localEv = DataSync::environmentVariable("NOPREFIX");
  if (localEv == "") {
    BOOST_CHECK_NE(DataSync::localWorkspacePrefix(), "");
  } else {
    BOOST_CHECK_EQUAL(DataSync::localWorkspacePrefix(), "");
  }
}

// @TODO runCommand_err_test

BOOST_AUTO_TEST_CASE(containsInThisOrder_test) {

  using DataSync::containsInThisOrder;

  const string input("0 one Two THREE");
  const vector<string> perfect = { "0", "one", "Two", "THREE" };
  const vector<string> partial = { "0", "one", "THR" };
  const vector<string> empty;
  const vector<string> unordered = { "0", "Two", "one", "THREE" };
  const vector<string> random = { "R", "and", "OM" };
  BOOST_CHECK(containsInThisOrder(input, perfect));
  BOOST_CHECK(containsInThisOrder(input, partial));
  BOOST_CHECK(containsInThisOrder(input, empty));
  BOOST_CHECK(not containsInThisOrder(input, unordered));
  BOOST_CHECK(not containsInThisOrder(input, random));
}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()
