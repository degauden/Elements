#ifndef EXAMPLEMODULE_DATASOURCEUSERTEMPLATEDMOCK_H
#define	EXAMPLEMODULE_DATASOURCEUSERTEMPLATEDMOCK_H

#include "ElementsKernel/EnableGMock.h"


using namespace testing;

namespace Elements {
namespace ElementsExamples {
/**
 * @class Euclid::ExampleModule::DataSourceUserTemplatedTypeMock
 *
 * @brief Mock object implementing the methods required to be used as
 * template type for
 * TemplatedDataSourceUser::sumRecords<T>(const T& data_source).
 */
class DataSourceUserTemplatedTypeMock {
public:

  MOCK_CONST_METHOD0(countRecords, size_t ());

  MOCK_CONST_METHOD1(getRecordValue, double (size_t));

};

}
}

#endif

