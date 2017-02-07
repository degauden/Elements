#ifndef EXAMPLEMODULE_DATASOURCEUSERTEMPLATEDMOCK_H
#define	 EXAMPLEMODULE_DATASOURCEUSERTEMPLATEDMOCK_H

#include "ElementsKernel/EnableGMock.h"

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

  virtual ~DataSourceUserTemplatedTypeMock()=default;

  MOCK_CONST_METHOD0(countRecords, std::size_t ());

  MOCK_CONST_METHOD1(getRecordValue, double (std::size_t));

};

}
}

#endif

