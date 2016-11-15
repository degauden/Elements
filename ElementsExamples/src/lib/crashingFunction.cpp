/**
 * @file crashingFunction.cpp
 *
 * @date Nov 15, 2016
 * @author hubert
 */

#include "ElementsExamples/crashingFunction.h"
#include "ElementsKernel/Logging.h"            // for the Logging

#include <stdexcept>                           // for the logic_error

namespace Elements {
namespace Examples {

void crashingFunction() {
  Logging logger = Logging::getLogger();
  logger.info() << "Entering Crashing Function";

  throw std::logic_error("arrg!");
}

} // namespace Examples
} // namespace Elements
