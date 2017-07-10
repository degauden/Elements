#ifndef ELEMENTS_EXAMPLES_PICALC_H__
#define ELEMENTS_EXAMPLES_PICALC_H__

#include "ElementsKernel/Export.h"

namespace Elements {
namespace Examples {

class ELEMENTS_API PiCalculator {
public:
  PiCalculator() = default;
  void calculate(unsigned int terms);

  typedef void (*show_result_callback_type)(double pi);
  void setShowResultCallback(show_result_callback_type f);

private:
  show_result_callback_type m_show_result_callback;
};

#endif

} // namespace Examples
} // namespace Elements
