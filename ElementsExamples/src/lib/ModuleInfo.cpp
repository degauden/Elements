/**
 * @file ElementsExamples/src/lib/ModuleInfo.cpp
 *
 * @date Feb 12, 2015
 * @author hubert
 */


#include "ElementsKernel/ThisModule.h"
#include "ElementsExamples/ModuleInfo.h"

namespace Elements {
namespace Examples {

const System::ModuleInfo& getModuleInfo() {

  return System::getThisModuleInfo();

}

} // namespace ElementsExamples
} // namespace Elements



