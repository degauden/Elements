#ifndef EXAMPLEMODULE_EXAMPLECLASS1_H
#define EXAMPLEMODULE_EXAMPLECLASS1_H

#include <utility>
#include "ElementsKernel/Export.h"



namespace Elements {

class ELEMENTS_API ExampleClass1{
public:
  ExampleClass1(double flux, double lambda);

  virtual ~ExampleClass1() = default;

  virtual std::pair<double,double> getRedshiftedValue(double z) const;

private:
  double m_flux;
  double m_lambda;
};


}
#endif
