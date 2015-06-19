#ifndef EXAMPLEMODULE_EXAMPLECLASS1MOCK_H
#define	EXAMPLEMODULE_EXAMPLECLASS1MOCK_H

#include "ElementsKernel/EnableGMock.h"

//using namespace testing;

namespace Elements {

class ExampleClass1Mock {
public:
  virtual ~ExampleClass1Mock() = default;
  MOCK_CONST_METHOD1(getRedshiftedValue, std::pair<double,double> (double));
};

}
#endif

