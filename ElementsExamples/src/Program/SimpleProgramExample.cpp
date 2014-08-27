/**
 * @file SimpleProgramExample.cpp
 *
 * @date Aug 27, 2014
 * @author Hubert Degaudenzi
 */

#include <iostream>

#include "ElementsKernel/SimpleProgram.h"

using namespace std;



namespace Elements {

class SimpleProgramExample : public SimpleProgram {

public:
  SimpleProgramExample() = default ;
  virtual ~SimpleProgramExample() = default ;

private:
  ExitCode main();
};


ExitCode SimpleProgramExample::main()
{
  ExitCode exit_code {ExitCode::OK};

  cout << "This works!!" << endl;

  return exit_code;
}


}  // namespace Elements

MAIN_FOR(Elements::SimpleProgramExample)
