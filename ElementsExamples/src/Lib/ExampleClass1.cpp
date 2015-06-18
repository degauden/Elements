#include <math.h>       /* pow */
#include "ElementsExamples/ExampleClass1.h"

namespace Elements {
  ExampleClass1::ExampleClass1(double flux, double lambda):
                m_flux{flux},
                m_lambda{lambda}{
  }

  std::pair<double,double> ExampleClass1 ::getRedshiftedValue(double z) const{
    auto z_plus_one = z+1;
    return std::make_pair(m_flux*pow(z_plus_one,-2),m_lambda*z_plus_one);
  }

}
