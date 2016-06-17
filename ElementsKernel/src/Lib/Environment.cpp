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

namespace Elements {


void Environment::Variable::operator=(const std::string& value) {
  m_env.m_old_values[m_index] = System::getEnv(m_index);
  System::setEnv(m_index, value);
}


Environment::Environment(): m_old_values{} {

}

Environment::~Environment() {

}

Environment::Variable Environment::operator[](const std::string& index) {
  return Environment::Variable(*this, index);
}

std::string Environment::operator[](const std::string index) const {
  return System::getEnv(index);
}


} // Elements namespace

