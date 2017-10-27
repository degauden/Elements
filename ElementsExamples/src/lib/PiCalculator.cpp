#include "ElementsExamples/PiCalculator.h"

namespace Elements {
namespace Examples {

void PiCalculator::calculate(unsigned int terms){
  double pi = 0.0;
  // Leibniz formula for pi
  double numerator = -1.0;
  double denominator = -1.0;
  for (unsigned int ii = 0; ii < terms; ++ii) {
    numerator   *= -1;
    denominator += 2.0;
    pi += numerator / denominator;
  }
  pi *= 4.0;
  m_show_result_callback(pi);
}

void PiCalculator::setShowResultCallback(show_result_callback_type f){
  m_show_result_callback = f;
}


} // namespace Examples
} // namespace Elements
