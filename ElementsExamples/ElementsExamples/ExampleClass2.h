#ifndef EXAMPLEMODULE_EXAMPLECLASS2_H
#define EXAMPLEMODULE_EXAMPLECLASS2_H

#include <map>
#include <vector>

#include "ElementsKernel/Export.h"


#include "ElementsExamples/ExampleClass1.h"

namespace Elements {

class ExampleClass2{
public:
  template<typename RedshifCalculator>
  ExampleClass2(const RedshifCalculator& redshift_calculator,
                const std::vector<double>& redshifts) {
    for (auto& redshift : redshifts) {
      m_redshifted_map.insert( std::make_pair(redshift, redshift_calculator.getRedshiftedValue(redshift)));
     }
  }

  virtual ~ExampleClass2() = default;

  ELEMENTS_API const std::map<double,std::pair<double,double>> & getRedshiftedMap() const ;

private:
  std::map<double,std::pair<double,double>> m_redshifted_map {};

};

}
#endif
