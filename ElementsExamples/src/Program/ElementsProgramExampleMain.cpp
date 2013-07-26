/**
 * @file ExampleMain.cpp
 * 
 * Created on: Jul 11, 2013
 * 
 * @author dubath
 *
 *
 * @section DESCRIPTION
 *
 * This class represents ...
 */

#include "ElementsExamples/ElementsProgramExample.h"

int main(int argc, const char* argv[]) {

  ElementsProgramExample epe{};

  epe.setup(argc, argv);

  return epe.pseudo_main();

} // Eof main()
