/**
 * @file ElementsKernel/Environment.h
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

#ifndef ELEMENTSKERNEL_ENVIRONMENT_H_
#define ELEMENTSKERNEL_ENVIRONMENT_H_

#include <map>                                   // for map
#include <string>                                // for string

#include "ElementsKernel/Export.h"               // for ELEMENTS_API
#include "ElementsKernel/System.h"               // for getEnv and setEnv

namespace Elements {

class ELEMENTS_API Environment {
public:

  class Variable {
  public:
    Variable(Environment& env, const std::string& index): m_env{env}, m_index{index} {}
    void operator=(const std::string&);
  private:
    Environment& m_env;
    std::string m_index;
  };

  Environment();
  ~Environment();

  Variable operator[](const std::string&);
  std::string operator[](const std::string) const;


private:

  std::map<std::string, std::string> m_old_values;

};


} // Elements namespace

#endif // ELEMENTSKERNEL_ENVIRONMENT_H_
