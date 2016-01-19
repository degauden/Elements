/**
 * @file TestMain.cpp
 *
 * @date Aug 7, 2014
 * @author hubert
 */

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

#include "ElementsKernel/Unused.h"


int main(ELEMENTS_UNUSED int argc, ELEMENTS_UNUSED char ** argv)
{
  CppUnit::TextUi::TestRunner runner;
  CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
  runner.addTest( registry.makeTest() );
  bool wasSuccessful = runner.run( "", false );
  return (wasSuccessful? 0 : 1 );
}
