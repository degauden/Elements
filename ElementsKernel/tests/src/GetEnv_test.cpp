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

#include "ElementsKernel/System.h"      // getEnv, isEnvSet

#include <iostream>                     // for cout, endl
#include <cstdlib>                      // for std::getenv
#include <string>                       // for string
#include <vector>

#include <boost/test/unit_test.hpp>

using std::string;
using std::getenv;                      // standard

BOOST_AUTO_TEST_SUITE(GetEnv_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(Raw_test) {

  string path_var {};
  char* tmp = getenv("PATH");
  if (tmp != NULL) {
    path_var = tmp;
  }

  // check that the variable exists
  BOOST_CHECK(tmp != NULL);
  // check that it is not empty
  BOOST_CHECK(not path_var.empty());

  string not_existing_path {};
  char* tmp_2 = getenv("Jlhjdji43k");
  if (tmp_2 != NULL) {
    not_existing_path = tmp_2;
  }

  // check that the variable doesn't exist
  BOOST_CHECK(tmp_2 == NULL);
  // the value is also empty
  BOOST_CHECK(not_existing_path.empty());

}

BOOST_AUTO_TEST_CASE(RawEmpty_test) {

  string var_name {"Dldoed7dja7c"};

  // create empty test env variable
  setenv(var_name.c_str(), "", 1);

  string path_var {};
  char* tmp = getenv(var_name.c_str());
  if (tmp != NULL) {
    path_var = tmp;
  }
  // check that the variable exists
  BOOST_CHECK(tmp != NULL);
  // check that it is empty
  BOOST_CHECK(path_var.empty());

  // clean-up
  unsetenv(var_name.c_str());

  // the env variable has ceased to be
  BOOST_CHECK(getenv(var_name.c_str()) == NULL);
}


BOOST_AUTO_TEST_CASE(StringWrap_test) {

  using Elements::System::getEnv;
  using Elements::System::isEnvSet;

  const string name_var {"PATH"};
  const string path_var = getenv("PATH");
  const string path_var2 = getEnv(name_var);
  const string path_var3 = getEnv("PATH");

  BOOST_CHECK_EQUAL(path_var, path_var2);
  BOOST_CHECK_EQUAL(path_var, path_var3);

  BOOST_CHECK(isEnvSet("PATH"));
  BOOST_CHECK(isEnvSet(name_var));

}

BOOST_AUTO_TEST_CASE(EmptyEnv_test) {

  using Elements::System::getEnv;

  string rnd_name {"Dldoed7dja7c"};

  const string name_var {"PATH"};
  string value_var {};

  // the variable exists
  BOOST_CHECK(getEnv(name_var, value_var));
  // and it is not empty
  BOOST_CHECK(not value_var.empty());

  // the variable doesn't exists
  BOOST_CHECK(not getEnv(rnd_name, value_var));
  // and its value is empty
  BOOST_CHECK(value_var.empty());

  // create empty test env variable
  setenv(rnd_name.c_str(), "", 1);
  // the variable exists
  BOOST_CHECK(getEnv(rnd_name, value_var));
  // and its value is empty
  BOOST_CHECK(value_var.empty());
  // destroy the empty test env variable
  unsetenv(rnd_name.c_str());

}

BOOST_AUTO_TEST_CASE(Set_test) {

  using Elements::System::setEnv;
  using Elements::System::isEnvSet;
  using Elements::System::getEnv;

  string rnd_name {"Dldoed7dja7c"};

  BOOST_CHECK(not isEnvSet(rnd_name));

  int r = setEnv(rnd_name, "");

  BOOST_CHECK(r == 0);
  BOOST_CHECK(isEnvSet(rnd_name));
  BOOST_CHECK(getEnv(rnd_name) == "");

  int r2 = setEnv(rnd_name, "toto", false);

  BOOST_CHECK(r2 == 0);
  BOOST_CHECK(isEnvSet(rnd_name));
  BOOST_CHECK(getEnv(rnd_name) == "");

  int r3 = setEnv(rnd_name, "titi");

  BOOST_CHECK(r3 == 0);
  BOOST_CHECK(isEnvSet(rnd_name));
  BOOST_CHECK(getEnv(rnd_name) == "titi");

}

BOOST_AUTO_TEST_CASE(UnSet_test) {

  using Elements::System::setEnv;
  using Elements::System::isEnvSet;
  using Elements::System::getEnv;
  using Elements::System::unSetEnv;

  string rnd_name {"Dldoed7dja7c"};

  int r = setEnv(rnd_name, "");

  BOOST_CHECK(r == 0);
  BOOST_CHECK(isEnvSet(rnd_name));
  BOOST_CHECK(getEnv(rnd_name) == "");

  int r2 = unSetEnv(rnd_name);
  BOOST_CHECK(r2 == 0);
  BOOST_CHECK(not isEnvSet(rnd_name));
  BOOST_CHECK(getEnv(rnd_name) == "");

}

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

