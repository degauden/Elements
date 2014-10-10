/**
 * @file SimpleProgramExample.cpp
 *
 * @date Aug 27, 2014
 * @author Hubert Degaudenzi
 */

#include <iostream>

#include "ElementsKernel/Exit.h"
#include "ElementsKernel/SimpleProgram.h"

using namespace std;



namespace Elements {

class SimpleProgramExample : public SimpleProgram {

public:
  SimpleProgramExample() = default ;
  virtual ~SimpleProgramExample() = default ;

private:
  ExitCode main();
  virtual void defineOptions();
};


ExitCode SimpleProgramExample::main()
{
  ExitCode exit_code {ExitCode::OK};

  cout << "This works!!" << endl;
  cout << getProgramName().c_str() << endl;
  cout << getProgramPath().string() << endl;

  return exit_code;
}

void SimpleProgramExample::defineOptions() {

}


}  // namespace Elements

MAIN_FOR(Elements::SimpleProgramExample)
