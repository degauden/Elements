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

#include "ElementsKernel/Environment.h"

#include <string>                            // for string
#include <sstream>                           // for stringstream
#include <stdexcept>                         // for out_of_range
#include <algorithm>                         // for find
#include <utility>                           // for move
#include <map>                               // for map

#include <boost/format.hpp>                  // for format

#include "ElementsKernel/System.h"           // for getEnv, setEnv, isEnvSet

using std::string;
using std::move;
using std::stringstream;
using std::ostream;
using std::endl;

namespace Elements {

using System::unSetEnv;
using System::setEnv;
using System::getEnv;
using System::isEnvSet;

Environment::Variable::Variable(Environment& env, const std::string& index): m_env(env), m_index(index) {
}

Environment::Variable::Variable(const Environment::Variable& other): m_env{other.m_env} {

  checkCompatibility(other);

}

Environment::Variable::Variable(Environment::Variable&& other): m_env{move(other.m_env)} {

  checkCompatibility(other);

}

Environment::Variable& Environment::Variable::operator=(const Environment::Variable& other) {

  checkCompatibility(other);

  m_env = other.m_env;

  return *this;
}

Environment::Variable& Environment::Variable::operator=(Environment::Variable&& other) {

  checkCompatibility(other);

  m_env = move(other.m_env);

  return *this;
}


Environment::Variable& Environment::Variable::operator=(const string& value) {

  set(value);

  return *this;
}

Environment::Variable& Environment::Variable::set(const string& value) {

  m_env.get().set(m_index, value);

  return *this;
}

Environment::Variable& Environment::Variable::unSet() {

  m_env.get().unSet(m_index);

  return *this;
}

Environment::Variable& Environment::Variable::append(const string& value) {

  m_env.get().append(m_index, value);

  return *this;
}

Environment::Variable& Environment::Variable::operator +=(const string& value) {

  return append(value);

}

Environment::Variable& Environment::Variable::prepend(const string& value) {

  m_env.get().prepend(m_index, value);

  return *this;
}

Environment::Variable Environment::Variable::operator+(const string& value) {

  Environment::Variable result(m_env, m_index);

  result.append(value);

  return result;
}


const string& Environment::Variable::index() const {
  return m_index;
}

Environment& Environment::Variable::env() const {
  return m_env;
}


string Environment::Variable::value() const {

  return m_env.get().get(m_index, "");

}

Environment::Variable::operator std::string() const {
  return value();
}

bool Environment::Variable::empty() const {
  return value().empty();
}

bool Environment::Variable::exists() const {
  return m_env.get().hasKey(m_index);
}

void Environment::Variable::checkCompatibility(const Environment::Variable& other) {

  if (m_index != other.m_index) {
    stringstream error_buffer;
    error_buffer << "The \"" << other.m_index << "\" environment variable"
                 << " cannot be copied to the \"" << m_index << "\" environment variable."
                 << endl;
    throw std::invalid_argument(error_buffer.str());
  }

}


//----------------------------------------------------------------------------

Environment::Environment(bool keep_same)
  : m_old_values{}, m_keep_same{keep_same}, m_added_variables{} {

}

Environment& Environment::restore() {

  for (const auto& v : m_added_variables) {
    unSetEnv(v);
  }

  for (const auto& v : m_old_values) {
    setEnv(v.first, v.second);
  }

  m_old_values = {};

  return *this;
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

Environment& Environment::set(const string& index, const string& value) {

  if (m_old_values.find(index) == m_old_values.end()) {
    if (hasKey(index)) {
      if ((not m_keep_same) || (getEnv(index) != value)) {
        m_old_values[index] = getEnv(index);
      }
    } else {
      m_added_variables.push_back(index);
    }
  }

  setEnv(index, value);

  return *this;

}

Environment& Environment::unSet(const string& index) {

  checkOutOfRange(index);

  if (m_old_values.find(index) == m_old_values.end()) {
    auto found_index = std::find(m_added_variables.begin(), m_added_variables.end(), index);
    if (found_index != m_added_variables.end()) {
      m_added_variables.erase(found_index);
    } else {
      m_old_values[index] = getEnv(index);
    }
  }

  unSetEnv(index);

  return *this;

}

Environment& Environment::append(const string& index, const string& value) {

  const string new_value = get(index) + value;

  set(index, new_value);

  return *this;
}

Environment& Environment::prepend(const string& index, const string& value) {

  const string new_value = value + get(index);

  set(index, new_value);

  return *this;
}

string Environment::get(const string& index, const string& default_value) const {
  string value {default_value};

  if (hasKey(index)) {
    value = getEnv(index);
  }

  return value;
}

bool Environment::hasKey(const string& index) {

  return isEnvSet(index);

}

void Environment::commit() {

  m_old_values = {};
  m_added_variables = {};

}

string Environment::generateScript(Environment::ShellType type) const {

  using std::map;
  using boost::format;

  stringstream script_text {};

  map<ShellType, string> set_cmd {{ShellType::sh, "export %s=%s"},
                                       {ShellType::csh, "setenv %s %s"}};
  map<ShellType, string> unset_cmd {{ShellType::sh, "unset %s"},
                                         {ShellType::csh, "unsetenv %s"}};

  for (const auto& v : m_old_values) {
    if (hasKey(v.first)) {
      script_text << format(set_cmd[type]) % v.first % get(v.first) << endl;
    } else {
      script_text << format(unset_cmd[type]) % v.first << endl;
    }
  }

  for (const auto& v : m_added_variables) {
    script_text << format(set_cmd[type]) % v % get(v) << endl;
  }

  return script_text.str();

}


void Environment::checkOutOfRange(const string& index) {

  if (not hasKey(index)) {
    stringstream error_buffer;
    error_buffer << "The environment doesn't contain the " << index << " variable." << endl;
    throw std::out_of_range(error_buffer.str());
  }

}

ostream& operator<<(ostream& stream, const Environment::Variable& v) {

  stream << v.value();

  return stream;

}

Environment::Variable operator+(const string& value, const Environment::Variable& other) {

  Environment::Variable result(other.env(), other.index());

  result.prepend(value);

  return result;

}


}  // namespace Elements
