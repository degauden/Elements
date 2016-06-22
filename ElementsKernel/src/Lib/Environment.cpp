/**
 * @file Environment.cpp
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

#include <string>                            // for string
#include <sstream>                           // for stringstream
#include <stdexcept>                         // for out_of_range
#include <algorithm>                         // for find

#include <boost/format.hpp>                  // for format

#include "ElementsKernel/System.h"           // for getEnv, setEnv, isEnvSet
#include "ElementsKernel/Environment.h"

using std::string;

namespace Elements {

void Environment::Variable::operator=(const string& value) {
  set(value);
}

void Environment::Variable::set(const string& value) {

  m_env.set(m_index, value);

}

void Environment::Variable::unSet() {

  m_env.unSet(m_index);

}

const string& Environment::Variable::index() const {
  return m_index;
}

string Environment::Variable::value() const {

  return m_env.get(m_index, "");

}

Environment::Variable::operator std::string() const {
  return value();
}

bool Environment::Variable::empty() const {
  return value().empty();
}

bool Environment::Variable::exists() const {
  return m_env.hasKey(m_index);
}

//----------------------------------------------------------------------------

Environment::Environment(bool keep_same)
  : m_old_values{}, m_keep_same{keep_same}, m_added_variables{} {

}

void Environment::restore() {

  for (const auto& v: m_added_variables) {
    System::unSetEnv(v);
  }

  for (const auto& v: m_old_values) {
    System::setEnv(v.first, v.second);
  }

  m_old_values = {};
}


Environment::~Environment() {
  restore();
}

Environment::Variable Environment::operator[](const string& index) {

  return Environment::Variable(*this, index);

}

const Environment::Variable Environment::operator[](const string& index) const {

  return Environment::Variable(const_cast<Environment&>(*this), index);

}

void Environment::set(const string& index, const string& value) {

  if (m_old_values.find(index) == m_old_values.end()) {
    if (hasKey(index)) {
      if ((not m_keep_same) or (System::getEnv(index) != value) ) {
        m_old_values[index] = System::getEnv(index);
      }
    } else {
      m_added_variables.push_back(index);
    }
  }

  System::setEnv(index, value);

}

void Environment::unSet(const string& index) {

  checkOutOfRange(index);

  if (m_old_values.find(index) == m_old_values.end()) {
    auto found_index = std::find(m_added_variables.begin(), m_added_variables.end(), index);
    if(found_index != m_added_variables.end()) {
      m_added_variables.erase(found_index);
    } else {
      m_old_values[index] = System::getEnv(index);
    }
  }

  System::unSetEnv(index);

}

string Environment::get(const string& index, const string& default_value) const {
  string value {default_value};

  if (hasKey(index)) {
    value = System::getEnv(index);
  }

  return value;
}

bool Environment::hasKey(const string& index){

  return System::isEnvSet(index);

}

void Environment::commit() {

  m_old_values = {};
  m_added_variables ={};

}

string Environment::generateScript(Environment::ShellType type) const {

  std::stringstream script_text {};

  std::map<ShellType, string> set_cmd {{ShellType::sh, "export %s=%s"},
                                       {ShellType::csh, "setenv %s %s"}};
  std::map<ShellType, string> unset_cmd {{ShellType::sh, "unset %s"},
                                         {ShellType::csh, "unsetenv %s"}};

  for (const auto& v: m_old_values) {
    if (hasKey(v.first)) {
      script_text << boost::format(set_cmd[type]) % v.first % get(v.first) << std::endl;
    } else {
      script_text << boost::format(unset_cmd[type]) % v.first << std::endl;
    }
  }

  for (const auto& v: m_added_variables) {
    script_text << boost::format(set_cmd[type]) % v % get(v) << std::endl;
  }

  return script_text.str();

}


void Environment::checkOutOfRange(const string& index) {

  if (not hasKey(index)) {
    std::stringstream error_buffer;
    error_buffer << "The environment doesn't contain the " << index << " variable." << std::endl;
    throw std::out_of_range(error_buffer.str());
  }

}


} // Elements namespace

