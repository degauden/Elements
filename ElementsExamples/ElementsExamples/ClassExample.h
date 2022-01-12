/**
 * @file ClassExample.h
 *
 * @date January 9, 2015
 * @author Pierre Dubath
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
 * @addtogroup ElementsExamples ElementsExamples
 * @{
 */

#ifndef ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_CLASSEXAMPLE_H_
#define ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_CLASSEXAMPLE_H_

#include <memory>  // for std::unique_ptr
#include <string>  // for std::string
#include <vector>  // for std::vector

#include <cstdint>  // for std::int64_t

#include "ElementsKernel/Export.h"

namespace Elements {
namespace Examples {

/**
 * @class ClassExample
 * @ingroup ElementsExamples
 * @brief
 *    An class example
 * @details
 *    Our naming convention and coding standard are used in this example
 * @startuml
 * class ClassExample {
 *  +ClassExample {static} factoryMethod()
 *  +std::string& {static} getStaticString()
 *  +double fundamentalTypeMethod(const double, const double) const
 *  -double m_ra
 *  -const std::string {static} s_static_string
 *  -std::int64_t m_source_id
 * }
 * @enduml
 */
class ELEMENTS_API ClassExample {

public:
  /**
   * @brief
   *    Example factory method
   * @details
   *
   * @param source_id
   *    The source identifier
   * @param ra
   *    Right Ascension coordinate
   *
   */
  static ClassExample factoryMethod(const std::int64_t source_id, const double ra);

  /*
   * Getter to access the static private string
   */
  static const std::string& getStaticString() {
    return s_static_string;
  }

  /**
   * @brief Destructor
   */
  virtual ~ClassExample() = default;

  /**
   * @brief
   *    Simple method example
   * @details
   *    This is a simple example method, taking a double variable in input and
   *    returning a double value. The displayed way to pass and return fundamental
   *    type variables is recommended as the cost of copying them is small.
   *
   *    The const keyword send a meaningful message to the user. The
   *    "input_variable" will not be modified and can safely be reused after
   *    the call to "fundamentalTypesMethod".
   *
   *    The syntax
   *
   *    output = methodName(input)
   *
   *    is strongly recommended. With C++11, the std::tuple can be used to return
   *    a set of values or objects.
   *
   * @param input_variable
   *    The input_variable double value
   *
   * @return
   *    The output of the method
   */
  double fundamentalTypeMethod(const double input_variable) const;

  /**
   * @brief
   *    Divide two double variables
   * @details
   *    This is a simple class to illustrate the case of a method which can
   *    throw an exception
   *
   * @param first
   *    The first double value
   * @param second
   *    The second double value
   * @return
   *    The division of the two double values
   * @throws
   *   EuclidException, if the second number is (close to) zero
   */
  double divideNumbers(const double first, const double second) const;

  /**
   * @brief
   *    Example method with a unique pointer argument
   * @details
   *    This illustrates the case where the ownership of the
   *    pointed object is transfered to the method. Users will have
   *    to move the object (syntax: move(p)) and cannot reused it
   *    afterwards in the calling code.
   *
   *    The vector is used here as an example object.
   *
   * @param vector_unique_ptr
   *    Unique pointer to a vector object
   */
  void passingUniquePointer(std::unique_ptr<std::vector<double>> vector_unique_ptr) const;

  /**
   * @brief
   *    Example method taking an object in input
   * @details
   *    General method example taking any object in input. Because a
   *    const reference is used, the method can accept both a rvalue or
   *    a rvalue as an argument.
   *
   *    The vector is used here as an example object.
   *
   * @param input_object
   *    a vector of double
   */
  void passingObjectInGeneral(const std::vector<double>& input_object) const;

  /**
   * Getter to access private sourceId
   */
  std::int64_t getSourceId() const {
    return m_source_id;
  }

  /**
   * Getter to access private m_ra
   *
   */
  double getRa() const {
    return m_ra;
  }

private:
  /**
   * @brief Constructor
   *
   * @details
   *
   * We propose a public factory and private constructor.
   * This is not really useful here, but could be interesting in
   * a more elaborated call especially if the factory is returning
   * an abstract class (interface) and that different implementation
   * can be chosen via the factory.
   *
   */
  ClassExample(const std::int64_t source_id, const double ra) : m_source_id(source_id), m_ra(ra) {}

  /// An example of a static string
  static const std::string s_static_string;

  /// Source ID as an example of a 64 bits integer
  std::int64_t m_source_id{0};

  /// Source right ascension
  double m_ra{0.0};
};

}  // namespace Examples
}  // namespace Elements

#endif  // ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_CLASSEXAMPLE_H_

/**@}*/
