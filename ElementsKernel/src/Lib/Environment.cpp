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
#include <sstream>                          // for stringstream
#include <stdexcept>                         // for out_of_range

#include "ElementsKernel/System.h"           // for getEnv, setEnv, isEnvSet
#include "ElementsKernel/Environment.h"

using std::string;

namespace Elements {

void Environment::Variable::operator=(const string& value) {
  set(value);
}

void Environment::Variable::set(const string& value) {

  if(m_env.m_old_values.find(m_index) == m_env.m_old_values.end()) {
    if(System::isEnvSet(m_index)) {
      if ((not m_env.m_keep_same) or (System::getEnv(m_index) != value) ) {
        m_env.m_old_values[m_index] = System::getEnv(m_index);
      }
    } else {
      m_env.m_added_variables.push_back(m_index);
    }
  }

  System::setEnv(m_index, value);

}

void Environment::Variable::unSet() {

  if(not System::isEnvSet(m_index)) {
    std::stringstream error_buffer;
    error_buffer << "The environment doesn't contain the " << m_index << " variable." << std::endl;
    throw std::out_of_range(error_buffer.str());
  }

  m_env.m_old_values[m_index] = System::getEnv(m_index);
  System::unSetEnv(m_index);

}

const string& Environment::Variable::index() const {
  return m_index;
}

string Environment::Variable::value() const {
  return System::getEnv(m_index);
}

Environment::Variable::operator std::string() const {
  return value();
}

bool Environment::Variable::empty() const {
  return value().empty();
}

bool Environment::Variable::exists() const {
  return System::isEnvSet(m_index);
}

Environment::Environment(bool keep_same): m_old_values{}, m_keep_same{keep_same}, m_added_variables{} {

}

void Environment::restore() {

  for(const auto& v: m_added_variables) {
    System::unSetEnv(v);
  }

  for(const auto& v: m_old_values) {
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


} // Elements namespace

