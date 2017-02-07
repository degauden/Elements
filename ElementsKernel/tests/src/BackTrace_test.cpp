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

#include <string>
#include <vector>
#include <boost/test/unit_test.hpp>

// Temporary includes for dev
#include <iostream>

void second(){

}

void first() {
  second();
}

BOOST_AUTO_TEST_SUITE(BackTrace_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Raw_test) {

  using std::size_t;
  using std::string;

  const size_t depth = 21;


  std::vector<string> trace = Elements::System::backTrace(depth);

  if(!trace.empty()) {
    size_t found = trace[0].find("BackTrace_test");
    BOOST_CHECK_NE(found, string::npos);
  }

}




//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

