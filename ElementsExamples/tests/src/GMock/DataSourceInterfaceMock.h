/**
 * @file DataSourceInterfaceMock.h
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

#ifndef ELEMENTSEXAMPLES_TESTS_SRC_GMOCK_DATASOURCEINTERFACEMOCK_H_
#define ELEMENTSEXAMPLES_TESTS_SRC_GMOCK_DATASOURCEINTERFACEMOCK_H_

#include "ElementsKernel/EnableGMock.h"
#include "ElementsExamples/DataSourceUser.h"

namespace Elements {
namespace Examples {
/**
 * @class Euclid::ExampleModule::DataSourceInterfaceMock
 *
 * @brief Mock object implementing the DataSourceInterface.
 */
class DataSourceInterfaceMock: public DataSourceInterface {
public:
  virtual ~DataSourceInterfaceMock() = default;

  MOCK_CONST_METHOD0(countRecords, std::size_t());

  MOCK_CONST_METHOD1(getRecordValue, double(std::size_t));


};

}  // namespace Examples
}  // namespace Elements

#endif  // ELEMENTSEXAMPLES_TESTS_SRC_GMOCK_DATASOURCEINTERFACEMOCK_H_

