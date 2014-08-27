/**
 * @file SimpleProgram.h
 *
 * @date Aug 27, 2014
 * @author Hubert Degaudenzi
 */

#ifndef ELEMENTSKERNEL_SIMPLEPROGRAM_H_
#define ELEMENTSKERNEL_SIMPLEPROGRAM_H_

#include "ElementsKernel/Main.h"


namespace Elements {

// Forward declaration. We don't need the definition right now.
enum class ExitCode;

class SimpleProgram {

public:

  ExitCode run(int argc, char** argv) noexcept;

protected:

  SimpleProgram() = default;
  virtual ~SimpleProgram() = default;

  virtual ExitCode main() = 0;

};

} // namespace Elements

#endif // ELEMENTSKERNEL_SIMPLEPROGRAM_H_
