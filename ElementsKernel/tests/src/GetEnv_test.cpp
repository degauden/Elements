/**
 * @file GetEnv_test.cpp
 *
 * @date Aug 27, 2015
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

#include <cstdlib>                      // for std::getenv

#include "ElementsKernel/System.h"

#include <string>
#include <vector>
#include <boost/test/unit_test.hpp>



using namespace std;



BOOST_AUTO_TEST_SUITE(GetEnv_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Raw_test) {


  string path_var = getenv("PATH");

  BOOST_CHECK(not path_var.empty());


}


BOOST_AUTO_TEST_CASE(StringWrap_test) {

  using Elements::System::getEnv;

  string name_var {"PATH"};
  string path_var = getenv("PATH");
  string path_var2 = getEnv(name_var);
  string path_var3 = getEnv("PATH");

  BOOST_CHECK_EQUAL(path_var, path_var2);
  BOOST_CHECK_EQUAL(path_var, path_var3);


}




//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

