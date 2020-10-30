/*
 * @file DataSourceUserTemplatedTypeMock.h
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

#ifndef ELEMENTSEXAMPLES_TESTS_SRC_GMOCK_DATASOURCEUSERTEMPLATEDTYPEMOCK_H_
#define ELEMENTSEXAMPLES_TESTS_SRC_GMOCK_DATASOURCEUSERTEMPLATEDTYPEMOCK_H_

#include <cstddef>
#include <gmock/gmock.h>

namespace Elements {
namespace Examples {
/**
 * @class Euclid::ExampleModule::DataSourceUserTemplatedTypeMock
 *
 * @brief Mock object implementing the methods required to be used as
 * template type for
 * TemplatedDataSourceUser::sumRecords<T>(const T& data_source).
 */
class DataSourceUserTemplatedTypeMock {
public:
  virtual ~DataSourceUserTemplatedTypeMock() = default;

  MOCK_CONST_METHOD0(countRecords, std::size_t());

  MOCK_CONST_METHOD1(getRecordValue, double(std::size_t));
};

}  // namespace Examples
}  // namespace Elements

#endif  // ELEMENTSEXAMPLES_TESTS_SRC_GMOCK_DATASOURCEUSERTEMPLATEDTYPEMOCK_H_
