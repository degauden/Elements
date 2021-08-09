/**
 * @file ElementsKernel/Environment.h
 * @brief Defines a class to handle the Environment
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

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_ENVIRONMENT_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_ENVIRONMENT_H_

#include <functional>  // for reference_wrapper
#include <iostream>    // for ostream
#include <map>         // for map
#include <string>      // for string
#include <vector>      // for vector

#include "ElementsKernel/Export.h"  // for ELEMENTS_API

namespace Elements {

/**
 * @brief Python dictionary-like Environment interface
 * @ingroup ElementsKernel
 */
class ELEMENTS_API Environment {
public:
  class Variable;

public:
  /// default constructor
  explicit Environment(bool keep_same = true);
  virtual ~Environment();

  Variable       operator[](const std::string&);
  const Variable operator[](const std::string& index) const;
  Environment&   restore();
  Environment&   set(const std::string&, const std::string&);
  Environment&   unSet(const std::string&);
  Environment&   append(const std::string&, const std::string&);
  Environment&   prepend(const std::string&, const std::string&);
  std::string    get(const std::string& index, const std::string& default_value = "") const;
  static bool    hasKey(const std::string&);
  void           commit();

  enum ShellType { sh, csh };

  std::string generateScript(ShellType) const;

private:
  /**
   * @brief check that the variable is in the environment
   * @ingroup ElementsKernel
   */
  static void checkOutOfRange(const std::string&);

  /// old value for changed variables
  std::map<std::string, std::string> m_old_values;

  bool m_keep_same;

  /// variable added to the environment
  std::vector<std::string> m_added_variables;
};

/**
 * @brief proxy class to overload the assignment
 * @ingroup ElementsKernel
 */
class Environment::Variable {

public:
  Variable() = delete;
  Variable(Environment& env, const std::string& index);
  Variable(const Variable& other);
  Variable(Variable&& other);
  Variable&          operator=(const Variable& other);
  Variable&          operator=(Variable&& other);
  Variable&          operator=(const std::string&);
  Variable&          set(const std::string&);
  Variable&          unSet();
  Variable&          append(const std::string&);
  Variable&          operator+=(const std::string&);
  Variable&          prepend(const std::string&);
  Variable           operator+(const std::string&);
  const std::string& index() const;
  Environment&       env() const;
  std::string        value() const;
  /// to string converter
       operator std::string() const;
  bool empty() const;
  bool exists() const;

private:
  void checkCompatibility(const Variable&);

  /// a copiable and movable reference
  std::reference_wrapper<Environment> m_env;

  /// The Name of the variable
  std::string m_index;
};

ELEMENTS_API std::ostream& operator<<(std::ostream&, const Environment::Variable&);

ELEMENTS_API Environment::Variable operator+(const std::string&, const Environment::Variable&);

}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_ENVIRONMENT_H_

/**@}*/
