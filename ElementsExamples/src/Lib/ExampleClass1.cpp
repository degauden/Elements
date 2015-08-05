#include <cmath>                               // for pow
#include "ElementsExamples/ExampleClass1.h"


namespace Elements {
  ExampleClass1::ExampleClass1(double flux, double lambda):
                m_flux{flux},
                m_lambda{lambda}{
  }

  std::pair<double,double> ExampleClass1::getRedshiftedValue(double z) const{
    double z_plus_one = z + 1.0;
    return std::make_pair(m_flux*std::pow(z_plus_one,-2),m_lambda*z_plus_one);
  }

}
