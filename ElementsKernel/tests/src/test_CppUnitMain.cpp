/*
 * test_CppUnitMain.cpp
 *
 *  Created on: Mar 26, 2013
 *      Author: hubert
 */
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/XmlOutputter.h>

// to produce one line per test
#include <cppunit/TestResult.h>

// needed by the implementation of cool::ProgressListener
#include <cppunit/TestFailure.h>
#include <cppunit/TestListener.h>

using namespace std;

namespace ElementsKernelTest {
// Copied from the COOL implementation
/** @class ProgressListener
 *
 *  Simple TestListener printing one line per test in the standard output.
 *
 *  Based on  CppUnit::BriefTestProgressListener (copy and paste)
 *  using cout instead of cerr.
 *
 *  @author Marco Clemencic
 *  @date   2006-11-13
 */
class ProgressListener: public CppUnit::TestListener {

public:

  /// Default constructor.
  ProgressListener() :
      m_lastTestFailed(false) {
  }

  /// Destructor.
  virtual ~ProgressListener() {
  }

  void startTest(CppUnit::Test *test) {
    cout << test->getName();
    cout.flush();
    m_lastTestFailed = false;
  }

  void addFailure(const CppUnit::TestFailure &failure) {
    cout << " : " << (failure.isError() ? "error" : "assertion");
    m_lastTestFailed = true;
  }

  void endTest(CppUnit::Test * /*test*/) {
    if (!m_lastTestFailed)
      cout << " : OK";
    cout << endl;
  }

private:

  bool m_lastTestFailed;

};

}

// Copied from the COOL implementation
#include <stdexcept>
int main(int argc, char* argv[]) {
  // Retrieve test path from command line first argument.
  // Default to "" which resolve to the top level suite.
  string testPath = (argc > 1) ? string(argv[1]) : string("");

  // Add a listener that collects test result
  //CppUnit::TestResultCollector result;
  //controller.addListener( &result );

  /// Get the top level suite from the registry
  CppUnit::Test *suite = CppUnit::TestFactoryRegistry::getRegistry().makeTest();

  /// Adds the test to the list of test to run
  // CppUnit::TestRunner runner;
  CppUnit::TextTestRunner runner;
  runner.addTest(suite);

  // Change the default outputter to a compiler error format outputter
  // uncomment the following line if you need a compiler outputter.
  runner.setOutputter(new CppUnit::CompilerOutputter(&runner.result(), cout));

  // Change the default outputter to a xml error format outputter
  // uncomment the following line if you need a xml outputter.
  //runner.setOutputter( new CppUnit::XmlOutputter( &runner.result(),
  //                                                    cout ) );

  runner.eventManager().addListener(new ElementsKernelTest::ProgressListener());

  //CppUnit::TestResultCollector *collector =
  //  new CppUnit::TestResultCollector();
  //runner.eventManager().addListener(collector);

  bool wasSuccessful = false;

  try {
    wasSuccessful = runner.run(testPath, false, true, false);
  }

  // Test path not resolved
  catch (invalid_argument &e) {
    cout << endl << "ERROR: " << e.what() << endl;
    return 0;
  }

  // Should never happen?
  catch (exception& e) {
    cout << endl << "UNEXPECTED STD EXCEPTION CAUGHT: " << e.what() << endl;
    return 0;
  }

  // Should never happen?
  catch (...) {
    cout << endl << "UNKNOWN EXCEPTION CAUGHT" << endl;
    return 0;
  }

  // Return error code 1 if the one of tests failed.
  int retcode = wasSuccessful ? 0 : 1;

  // Uncomment the next line if you want to integrate CppUnit with Oval
  // cout << "[OVAL] Cppunit-result =" << retcode << endl;
  return retcode;

}

