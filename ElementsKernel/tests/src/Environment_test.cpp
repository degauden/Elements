/**
 * @file Environment_test.cpp
 *
 * @date Jun 17, 2016
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

#include <iostream>                            // for interactive testing
#include <string>
#include <stdexcept>                           // for out_of_range

#include <boost/test/unit_test.hpp>

#include "ElementsKernel/System.h"             // for isEnvSet
#include "ElementsKernel/Environment.h"        // for Environment

using std::string;
using Elements::Environment;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

struct Environment_fixture {

  const Environment base;

  Environment_fixture() {
    // setup
  }
  ~Environment_fixture() {
    // teardown
  }
};

BOOST_AUTO_TEST_SUITE(Environment_test)

//-----------------------------------------------------------------------------

BOOST_FIXTURE_TEST_CASE(Base_test, Environment_fixture) {

   BOOST_CHECK(not base["PATH"].empty());
   BOOST_CHECK(base["hkedis_4"].empty());

   BOOST_CHECK(base["PATH"].exists());
   BOOST_CHECK(not base["hkedis_4"].exists());

}

BOOST_AUTO_TEST_CASE(SetVariable_test) {

  using Elements::System::isEnvSet;
  using Elements::System::getEnv;
  using Elements::System::unSetEnv;

  Environment First;

  const string var_name {"LKkhdfk4lad"};

  BOOST_CHECK(not isEnvSet(var_name));

  First[var_name] = "toto";

  BOOST_CHECK(isEnvSet(var_name));
  BOOST_CHECK(getEnv(var_name) == string(First[var_name]));

  unSetEnv(var_name);

  const string var_name_2 {"LKehdfw4lad"};

  BOOST_CHECK(not isEnvSet(var_name_2));

  First[var_name_2] = "";

  BOOST_CHECK(isEnvSet(var_name_2));
  BOOST_CHECK("" == string(First[var_name_2]));

  unSetEnv(var_name_2);

}

BOOST_AUTO_TEST_CASE(UnSetVariable_test) {

  using Elements::System::isEnvSet;
  using Elements::System::getEnv;

  Environment First;

  const string var_name {"LKkhdfk4lad"};

  BOOST_CHECK(not isEnvSet(var_name));

  First[var_name] = "toto";

  BOOST_CHECK(isEnvSet(var_name));
  BOOST_CHECK(getEnv(var_name) == string(First[var_name]));

  First[var_name].unSet();

  BOOST_CHECK(not isEnvSet(var_name));

  BOOST_CHECK_THROW(First["jdldaoociej"].unSet(), std::out_of_range);

}

BOOST_AUTO_TEST_CASE(SubEnv_test) {

  using Elements::System::isEnvSet;
  using Elements::System::getEnv;

  const string var_name {"dikeZdhjdSHD"};

  Environment First;

  BOOST_CHECK(not First[var_name].exists());
  BOOST_CHECK(not isEnvSet(var_name));

  {

    BOOST_CHECK(not First[var_name].exists());
    BOOST_CHECK(not isEnvSet(var_name));

    Environment Second;

    Second[var_name] = "toto";
    BOOST_CHECK(Second[var_name].exists());
    BOOST_CHECK(isEnvSet(var_name));
    BOOST_CHECK(getEnv(var_name) == "toto");

    {
      Environment Third;
      Third[var_name] = "titi";
      BOOST_CHECK(Third[var_name].exists());
      BOOST_CHECK(isEnvSet(var_name));
      BOOST_CHECK(getEnv(var_name) == "titi");
    }

    BOOST_CHECK(Second[var_name].exists());
    BOOST_CHECK(isEnvSet(var_name));
    BOOST_CHECK(getEnv(var_name) == "toto");

    Second[var_name] = "toto";
    BOOST_CHECK(Second[var_name].exists());
    BOOST_CHECK(isEnvSet(var_name));
    BOOST_CHECK(getEnv(var_name) == "toto");


  }

  BOOST_CHECK(not First[var_name].exists());
  BOOST_CHECK(not isEnvSet(var_name));

}


BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------
