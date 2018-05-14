/**
 * @file DataSourceUser.h
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

#ifndef ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_DATASOURCEUSER_H_
#define ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_DATASOURCEUSER_H_

#include  <cstdlib>                 // for std::size_t

#include "ElementsKernel/Export.h"  // For ELEMENTS_API

namespace Elements {
namespace Examples {

/**
 * @class DataSourceInterface
 * @brief This class has been created to demonstrate unit testing. It is an
 * Interface over a DataSource (a File, a DataBase Table, ...).
 *
 * @details
 * Note that as we test the call to this interface, no implementation is provided.
 */
class DataSourceInterface {

public:
  /**
   * @brief Count the number of records into the DataSource.
   *
   * @return
   * A unsigned integer containing the number of records in the DataSource.
   */
  virtual std::size_t countRecords() const = 0;

  /**
   * @brief Fetch the value of the n-th record of the DataSource.
   *
   * @param index
   * The zero-based index of the record into the DataSource.
   *
   * @return The value of the record at the provided index.
   *
   * @throw Elements::Exception
   * If no record exists at the specified index.
   */
  virtual double getRecordValue(std::size_t index) const = 0;

  virtual ~DataSourceInterface() = default;
};

/**
 * @class DataSourceUser
 * @brief This class has been created to demonstrate unit testing. It manipulates
 * an object implementing DataSourceInterface.
 */
class ELEMENTS_API DataSourceUser {

public:
  /**
   * @brief Compute the sum of the values of the records stored into the provided
   * DataSource
   *
   * @param data_source
   * A reference to an object implementing DataSourceInterface.
   *
   * @return The sum of the values of the records into the DataSource object
   */
  double sumRecords(const DataSourceInterface& data_source);

};

}  // namespace Examples
}  // namespace Elements

#endif  // ELEMENTSEXAMPLES_ELEMENTSEXAMPLES_DATASOURCEUSER_H_

/**@}*/
