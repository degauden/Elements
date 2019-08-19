/**
 * @file TemplatedDataSourceUser.h
 *
 * @date Sep 23, 2015
 * @author Florian Dubath
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

#ifndef ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_TEMPLATEDDATASOURCEUSER_H_
#define ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_TEMPLATEDDATASOURCEUSER_H_

namespace Elements {
namespace Examples {

/**
 * @class TemplatedDataSourceUser
 * @ingroup ElementsExamples
 * @brief This class has been created to demonstrate unit testing. It manipulate
 * an object representing a DataSource using template to achieve polymorphism.
 */
class TemplatedDataSourceUser{
public:

  /**
     * @brief Compute the sum of the values of the records stored into the provided
     * DataSource
     *
     * @tparam T
     * A type representing a DataSource. The type must declare the methods
     * + size_t countRecords() const
     * + double getRecordValue(size_t index) const
     *
     * @param data_source
     * A reference to a DataSource object.
     *
     * @return The sum of the values of the records into the DataSource object
     */
  template <typename T>
  double sumRecords(const T& data_source);
};

}  // namespace Examples
}  // namespace Elements

#include "ElementsExamples/_impl/TemplatedDataSourceUser.icpp"

#endif  // ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_TEMPLATEDDATASOURCEUSER_H_

/**@}*/
