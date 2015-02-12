/**
 * @file ElementsExamples/src/Lib/ModuleInfo.cpp
 *
 * @date Feb 12, 2015
 * @author hubert
 */


#include "ElementsKernel/ThisModule.h"
#include "ElementsExamples/ModuleInfo.h"

namespace Elements {

const System::ModuleInfo& getModuleInfo() {

  return System::getThisModuleInfo();

}

} // namespace Elements


