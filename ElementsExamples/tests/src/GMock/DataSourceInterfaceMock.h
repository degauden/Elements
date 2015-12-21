#ifndef EXAMPLEMODULE_DATASOURCEINTERFACEMOCK_H
#define	 EXAMPLEMODULE_DATASOURCEINTERFACEMOCK_H

#include "ElementsKernel/EnableGMock.h"
#include "ElementsExamples/DataSourceUser.h"

using namespace testing;

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

  MOCK_CONST_METHOD0(countRecords, size_t ());

  MOCK_CONST_METHOD1(getRecordValue, double (size_t));


};

}
}

#endif

