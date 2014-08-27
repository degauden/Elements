/**
 * @file SimpleProgram.h
 *
 * @date Aug 27, 2014
 * @author Hubert Degaudenzi
 */

#ifndef ELEMENTSKERNEL_SIMPLEPROGRAM_H_
#define ELEMENTSKERNEL_SIMPLEPROGRAM_H_

#include "ElementsKernel/Exit.h"
#include "ElementsKernel/Export.h" // ELEMENTS_API

#include "ElementsKernel/Main.h"


namespace Elements {

class SimpleProgram {
public:
  SimpleProgram();
  virtual ~SimpleProgram();
};

} // namespace Elements

#endif // ELEMENTSKERNEL_SIMPLEPROGRAM_H_
