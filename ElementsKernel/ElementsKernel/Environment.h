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

#include <iostream>                              // for ostream
#include <map>                                   // for map
#include <string>                                // for string
#include <vector>                                // for vector

#include "ElementsKernel/Export.h"               // for ELEMENTS_API

namespace Elements {

/*
 * @brief Python dictionary-like Environment interface
 */
class ELEMENTS_API Environment {
public:
  /**
   * @brief proxy class to overload the assignment
   */
  class Variable {
  public:
    Variable(Environment& env, const std::string& index): m_env(env), m_index(index) {}
    void operator=(const std::string&);
    void set(const std::string&);
    void unSet();
    const std::string& index() const;
    std::string value() const;
    /// to string converter
    operator std::string() const;
    bool empty() const;
    bool exists() const;
  private:
    Environment& m_env;
    std::string m_index;
  };

  Environment(bool keep_same=true);
  ~Environment();

  Variable operator[](const std::string&);
  const Variable operator[](const std::string& index) const;
  void restore();
  void set(const std::string&, const std::string&);
  void unSet(const std::string&);
  std::string get(const std::string& index, const std::string& default_value="") const;
  static bool hasKey(const std::string&);
  void commit();

  enum ShellType {sh, csh};

  std::string generateScript(ShellType) const;

private:

  static void checkOutOfRange(const std::string&);

  std::map<std::string, std::string> m_old_values;
  bool m_keep_same;
  std::vector<std::string> m_added_variables;

};

ELEMENTS_API inline std::ostream& operator<<(std::ostream& stream, const Environment::Variable& v) {
  stream << v.value();
  return stream;
}


} // Elements namespace

#endif // ELEMENTSKERNEL_ENVIRONMENT_H_
