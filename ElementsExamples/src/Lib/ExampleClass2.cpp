#include "ElementsExamples/ExampleClass2.h"

namespace Elements {


const std::map<double,std::pair<double,double>> & ExampleClass2::getRedshiftedMap() const {
  return m_redshifted_map;
}

}
