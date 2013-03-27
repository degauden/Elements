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
 *  using std::cout instead of std::cerr.
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
    cout << std::endl;
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
  std::string testPath = (argc > 1) ? std::string(argv[1]) : std::string("");

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
  runner.setOutputter(
      new CppUnit::CompilerOutputter(&runner.result(), std::cout));

  // Change the default outputter to a xml error format outputter
  // uncomment the following line if you need a xml outputter.
  //runner.setOutputter( new CppUnit::XmlOutputter( &runner.result(),
  //                                                    std::cout ) );

  runner.eventManager().addListener(new ElementsKernelTest::ProgressListener());

  //CppUnit::TestResultCollector *collector =
  //  new CppUnit::TestResultCollector();
  //runner.eventManager().addListener(collector);

  bool wasSuccessful = false;

  try {
    wasSuccessful = runner.run(testPath, false, true, false);
  }

  // Test path not resolved
  catch (std::invalid_argument &e) {
    std::cout << std::endl << "ERROR: " << e.what() << std::endl;
    return 0;
  }

  // Should never happen?
  catch (std::exception& e) {
    std::cout << std::endl << "UNEXPECTED STD EXCEPTION CAUGHT: " << e.what()
        << std::endl;
    return 0;
  }

  // Should never happen?
  catch (...) {
    std::cout << std::endl << "UNKNOWN EXCEPTION CAUGHT" << std::endl;
    return 0;
  }

  // Return error code 1 if the one of tests failed.
  int retcode = wasSuccessful ? 0 : 1;

  // Uncomment the next line if you want to integrate CppUnit with Oval
  // std::cout << "[OVAL] Cppunit-result =" << retcode << std::endl;
  return retcode;

}

