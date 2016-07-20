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
#include <boost/regex.hpp>                     // for regex, regex_match

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


class ThatClass {
public:
  ThatClass(): m_env{}, m_internal_var_name{"duhfbs"}, m_internal_var_value{"titi"} {
    m_env[m_internal_var_name] = m_internal_var_value;
  }
  void setEnv(const string& var_name, const string& var_value) {
    m_env[var_name] = var_value;
  }
  string getEnv(const string& var_name) const {
    return m_env[var_name];
  }

  bool checkInternalEnv() const {
    using Elements::System::getEnv;
    return (getEnv(m_internal_var_name) == m_internal_var_value);
  }
private:
  Environment m_env;
  string m_internal_var_name;
  string m_internal_var_value;
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

  Environment first;

  const string var_name {"LKkhdfk4lad"};

  BOOST_CHECK(not isEnvSet(var_name));

  first[var_name] = "toto";

  BOOST_CHECK(isEnvSet(var_name));
  BOOST_CHECK(getEnv(var_name) == string(first[var_name]));

  unSetEnv(var_name);

  const string var_name_2 {"LKehdfw4lad"};

  BOOST_CHECK(not isEnvSet(var_name_2));

  first[var_name_2] = "";

  BOOST_CHECK(isEnvSet(var_name_2));
  BOOST_CHECK("" == string(first[var_name_2]));

  unSetEnv(var_name_2);

}

BOOST_AUTO_TEST_CASE(UnSetVariable_test) {

  using Elements::System::isEnvSet;
  using Elements::System::getEnv;

  Environment first;

  const string var_name {"LKkhdfk4lad"};

  BOOST_CHECK(not isEnvSet(var_name));

  first[var_name] = "toto";

  BOOST_CHECK(isEnvSet(var_name));
  BOOST_CHECK(getEnv(var_name) == string(first[var_name]));

  first[var_name].unSet();

  BOOST_CHECK(not isEnvSet(var_name));

  BOOST_CHECK_THROW(first["jdldaoociej"].unSet(), std::out_of_range);

}

BOOST_AUTO_TEST_CASE(SubEnv_test) {

  using Elements::System::isEnvSet;
  using Elements::System::getEnv;

  const string var_name {"dikeZdhjdSHD"};

  Environment first;

  BOOST_CHECK(not first[var_name].exists());
  BOOST_CHECK(not isEnvSet(var_name));

  {

    BOOST_CHECK(not first[var_name].exists());
    BOOST_CHECK(not isEnvSet(var_name));

    Environment second;

    second[var_name] = "toto";
    BOOST_CHECK(second[var_name].exists());
    BOOST_CHECK(isEnvSet(var_name));
    BOOST_CHECK(getEnv(var_name) == "toto");

    {
      Environment third;
      third[var_name] = "titi";
      BOOST_CHECK(third[var_name].exists());
      BOOST_CHECK(isEnvSet(var_name));
      BOOST_CHECK(getEnv(var_name) == "titi");
    }

    BOOST_CHECK(second[var_name].exists());
    BOOST_CHECK(isEnvSet(var_name));
    BOOST_CHECK(getEnv(var_name) == "toto");

    second[var_name] = "toto";
    BOOST_CHECK(second[var_name].exists());
    BOOST_CHECK(isEnvSet(var_name));
    BOOST_CHECK(getEnv(var_name) == "toto");


  }

  BOOST_CHECK(not first[var_name].exists());
  BOOST_CHECK(not isEnvSet(var_name));

}

BOOST_AUTO_TEST_CASE(NestedSet_test) {

  using Elements::System::isEnvSet;
  using Elements::System::setEnv;
  using Elements::System::getEnv;

  const string var_name {"ddTdh_lds"};

  const string var_name_2 {"ddTdh_sad4ds"};
  const string var_value_2 {"bla"};
  setEnv(var_name_2, var_value_2);
  BOOST_CHECK(isEnvSet(var_name_2));
  BOOST_CHECK(getEnv(var_name_2) == var_value_2);

  const string var_name_3 {"ddSUETdh_sad4ds"};
  setEnv(var_name_3, "alpha");
  BOOST_CHECK(isEnvSet(var_name_3));
  BOOST_CHECK(getEnv(var_name_3) == "alpha");


  {
    BOOST_CHECK(not isEnvSet(var_name));

    BOOST_CHECK(isEnvSet(var_name_2));
    BOOST_CHECK(getEnv(var_name_2) == var_value_2);

    Environment local;
    local[var_name] = "toto";
    BOOST_CHECK(isEnvSet(var_name));

    const string var_value_3 {"blu"};
    local[var_name_2] = var_value_3;

    BOOST_CHECK(isEnvSet(var_name_2));
    BOOST_CHECK(getEnv(var_name_2) == var_value_3);

    local.unSet(var_name_3);
    BOOST_CHECK(not isEnvSet(var_name_3));

  }

  BOOST_CHECK(not isEnvSet(var_name));

  BOOST_CHECK(isEnvSet(var_name_2));
  BOOST_CHECK(getEnv(var_name_2) == var_value_2);

  BOOST_CHECK(isEnvSet(var_name_3));
  BOOST_CHECK(getEnv(var_name_3) == "alpha");

}

BOOST_AUTO_TEST_CASE(GenScript_test) {

  using boost::regex;
  using boost::regex_match;

  Environment first;

  first["blad3"] = "djjsd/d:";

  const string sh_script_text = first.generateScript(Environment::ShellType::sh);
  regex sh_set_rule {"\\s*export\\s+blad3=djjsd/d:\\s*$"};
  BOOST_CHECK(regex_match(sh_script_text, sh_set_rule));

  const string csh_script_text = first.generateScript(Environment::ShellType::csh);
  regex csh_set_rule {"\\s*setenv\\s+blad3\\s+djjsd/d:\\s*$"};
  BOOST_CHECK(regex_match(csh_script_text, csh_set_rule));


}

BOOST_AUTO_TEST_CASE(Commit_test) {

  using Elements::System::isEnvSet;

  const string var_name {"ddTdh_lds"};

  BOOST_CHECK(not isEnvSet(var_name));

  {
    Environment local;
    local[var_name] = "toto";
    BOOST_CHECK(isEnvSet(var_name));
    local.commit();
  }

  BOOST_CHECK(isEnvSet(var_name));
}

BOOST_AUTO_TEST_CASE(Class_test) {

  using Elements::System::getEnv;

  const string var_name = "ddTdh_lds";
  const string var_value = "toto";

  ThatClass that;

  that.setEnv(var_name, var_value);

  BOOST_CHECK(var_value == that.getEnv(var_name));
  BOOST_CHECK(getEnv(var_name) == var_value);

  BOOST_CHECK(that.checkInternalEnv());

}

BOOST_AUTO_TEST_CASE(Append_test) {

  Environment local;

  const string added_value = ":ddgfhdf";

  const string original_path = local["PATH"];

  local.append("PATH", added_value);

  BOOST_CHECK(local["PATH"].value() == original_path + added_value);


}

BOOST_AUTO_TEST_CASE(Prepend_test) {

  Environment local;

  const string added_value = ":ddgfhdf";

  const string original_path = local["PATH"];

  local.prepend("PATH", added_value);

  BOOST_CHECK(local["PATH"].value() == added_value + original_path);


}


BOOST_AUTO_TEST_CASE(OperatorPlusEqual_test) {

  Environment local;

  const string added_value = ":ddgfhdf";

  const string original_path = local["PATH"];

  local["PATH"] += added_value;

  BOOST_CHECK(local["PATH"].value() == original_path + added_value);


}

BOOST_AUTO_TEST_CASE(OperatorPlus_test) {

  Environment local;

  const string added_value = ":ddgfhdf";

  const string original_path = local["PATH"];

  local["PATH"] = local["PATH"] + added_value;

  BOOST_CHECK(local["PATH"].value() == original_path + added_value);

}

BOOST_AUTO_TEST_CASE(OperatorPlusOther_test) {

  Environment local;

  const string added_value = ":ddgfhdf";

  const string original_path = local["PATH"];

  local["PATH"] = added_value + local["PATH"];

  BOOST_CHECK(local["PATH"].value() == added_value + original_path);

}

BOOST_AUTO_TEST_CASE(CheckCompatibility_test) {

  Environment local;

  local["dkdd"] = "toto";
  local["edkkd"] = "tata";

  BOOST_CHECK_THROW(local["dkdd"] = local["edkkd"], std::invalid_argument);

}


BOOST_AUTO_TEST_SUITE_END()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------
