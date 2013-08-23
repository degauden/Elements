/**
 * @file FileLocator_test.cpp
 *
 * Created on: Aug 7, 2013
 *     Author: Pavel Binko
 */

#include <string>
#include <vector>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

namespace fs = boost::filesystem;

#include "ElementsKernel/FileLocator.h"
#include "ElementsKernel/ElementsException.h"

using namespace std;

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//
// Begin of the Boost tests
//
//-----------------------------------------------------------------------------

struct FileLocator_Fixture {

  string envVariable = "ELEMENTS_CONF_PATH";
  string extension = ".conf";

  string rootPath = getenv("ELEMENTSKERNELROOT");
  fs::path fullPath_path = fs::path{rootPath} / "tests/conf";
  string fullPath = fullPath_path.string();
  string composedPath = "/opt/local/bin:" + fullPath + ":/bin";

  FileLocator_Fixture() {
    // setup
  }
  ~FileLocator_Fixture() {
    // teardown
  }
};

BOOST_AUTO_TEST_SUITE (FileLocator_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(searchFileInPath_test) {

  string envVariable = "ELEMENTS_CONF_PATH";

  string extension = ".conf";

  string rootPath = getenv("ELEMENTSKERNELROOT");
  string fullPath = rootPath + "/tests/conf/";
  string composedPath = "/opt/local/bin:" + fullPath + ":/bin";
  //cout << composedPath << endl;
  setenv(envVariable.c_str(), composedPath.c_str(), 1);
  fs::path file { "MockFileForTestingFileLocator_3.conf" };
  fs::path actualFullPath = searchFileInPath(envVariable, file.stem(),
      extension);
  //cout << "From test 2 : File = " << actualFullPath << endl;
  fs::path expectedFullPath = fullPath / file;
  //BOOST_CHECK(actualFullPath.empty());
  BOOST_CHECK(actualFullPath == expectedFullPath);

}  // Eof searchFileInPath_1 test

BOOST_AUTO_TEST_CASE(getPathLastSegment_test) {

  string rootPath = getenv("ELEMENTSKERNELROOT");
  fs::path testPath { rootPath
      + "tests/conf/ElementsKernel/MockFileForTestingFileLocator.conf" };
  BOOST_CHECK(
      getPathLastSegment(testPath).string() == "ElementsKernel");
  fs::path testPath2 { rootPath
      + "tests/conf/ElementsKernel/MockFileForTestingFileLocator" };
  BOOST_CHECK(
      getPathLastSegment(testPath2).string() == "ElementsKernel");

  fs::path testPath3 { "MockFileForTestingFileLocator" };
  BOOST_CHECK(getPathLastSegment(testPath3).empty());
  fs::path testPath4 { "MockFileForTestingFileLocator.conf" };
  BOOST_CHECK(getPathLastSegment(testPath4).empty());
  fs::path testPath5 { "" };
  BOOST_CHECK(getPathLastSegment(testPath5).empty());

}

/*
 * There are 3 configuration files named "MockConfFileForTestingFileLocator_4.conf" in
 * the test conf sub-directories
 */
BOOST_FIXTURE_TEST_CASE(searchFileRecursivelyInDir_test, FileLocator_Fixture) {
  vector<fs::path> results = searchFileRecursivelyInDir(fullPath,
      "MockConfFileForTestingFileLocator_4.conf");
  BOOST_CHECK(results.size() == 3);
  results = searchFileRecursivelyInDir(fullPath,
      "MockConfFileForTestingFileLocator_4");
  BOOST_CHECK(results.empty());
  results = searchFileRecursivelyInDir(fullPath, "");
  BOOST_CHECK(results.empty());
}

/*
 * Only two of the three "MockConfFileForTestingFileLocator_4.conf" have a correct prefix
 *
 * There is one file "MockConfFileForTestingFileLocator_5.conf" but it does not have a correct prefix
 */
BOOST_FIXTURE_TEST_CASE(checkPathPrefix_test, FileLocator_Fixture) {

  vector<fs::path> results;
  results = searchFileRecursivelyInDir(fullPath,
      "MockConfFileForTestingFileLocator_4.conf");
  BOOST_CHECK(results.size() == 3);
  results = checkPathPrefix(results, "");
  BOOST_CHECK(results.size() == 3);
  results = checkPathPrefix(results, "ElementsKernel");
  BOOST_CHECK(results.size() == 2);
  results = searchFileRecursivelyInDir(fullPath,
      "MockConfFileForTestingFileLocator_5.conf");
  BOOST_CHECK(results.size() == 1);
  results = checkPathPrefix(results, "ElementsKernel");
  BOOST_CHECK(results.empty());
}

/*
 * For the time being, this return an element if there is onel one element in the vector and
 * returns an expection otherwise
 */
BOOST_FIXTURE_TEST_CASE(selectFileInVector_test_ok, FileLocator_Fixture) {
  fs::path result;
  vector<fs::path> files;
  files = searchFileRecursivelyInDir(fullPath,
      "MockConfFileForTestingFileLocator_1.conf");
  BOOST_CHECK(files.size() == 1);
  vector<fs::path> checkedFiles = checkPathPrefix(files, "ElementsKernel");
  result = selectFileInVector(checkedFiles);
  fs::path actualResult = result.filename();
  fs::path expectedResult { "MockConfFileForTestingFileLocator_1.conf" };
  BOOST_CHECK(actualResult == expectedResult);

}

BOOST_FIXTURE_TEST_CASE(selectFileInVector_test_exception, FileLocator_Fixture) {

  fs::path result;
  vector<fs::path> files;
  files = searchFileRecursivelyInDir(fullPath,
      "MockConfFileForTestingFileLocator_4.conf");
  BOOST_CHECK(files.size() == 3);
  files = checkPathPrefix(files, "ElementsKernel");
  BOOST_CHECK(files.size() == 2);

    bool exception = false;
     try {
       result = selectFileInVector(files);
     } catch (const ElementsException & e) {
       //exception = true;
       string exception_str = e.what();
       exception =
           (exception_str.find(
               "Multiple configuration files detected")
               != string::npos);
     }
     BOOST_CHECK(exception);
}


BOOST_FIXTURE_TEST_CASE(searchConfFileInPathVariable_1_up, FileLocator_Fixture) {

  setenv(envVariable.c_str(), composedPath.c_str(), 1);
  string file { "MockConfFileForTestingFileLocator_1.conf" };
  fs::path actualFullPath = searchConfFileInPathVariable(file);
  fs::path expectedFullPath = fullPath / fs::path { "ElementsKernel" } / fs::path { file };
  BOOST_CHECK(actualFullPath == expectedFullPath);

}  // Eof searchConfFileInPath_1 up

BOOST_FIXTURE_TEST_CASE(searchConfFileInPathVariable_2_down1, FileLocator_Fixture) {

  setenv(envVariable.c_str(), composedPath.c_str(), 1);
  string file { "MockConfFileForTestingFileLocator_2" };
  fs::path actualFullPath = searchConfFileInPathVariable(file);
  fs::path expectedFullPath = fullPath / fs::path { "additionalDirForTesting/ElementsKernel" } / fs::path{file}.replace_extension(extension) ;
  BOOST_CHECK(actualFullPath == expectedFullPath);

}

BOOST_FIXTURE_TEST_CASE(searchConfFileInPath_3_wrongParent, FileLocator_Fixture) {

  setenv(envVariable.c_str(), composedPath.c_str(), 1);
  string file { "MockConfFileForTestingFileLocator_3" };
  fs::path actualFullPath = searchConfFileInPathVariable(file);
  BOOST_CHECK(actualFullPath.empty());

}

BOOST_FIXTURE_TEST_CASE(searchConfFileInPath_wrongFile, FileLocator_Fixture) {

  setenv(envVariable.c_str(), composedPath.c_str(), 1);
  string file { "Toto" };
  fs::path actualFullPath = searchConfFileInPathVariable(file);
  BOOST_CHECK(actualFullPath.empty());

}

BOOST_FIXTURE_TEST_CASE(searchConfFileInPath_twoFilesException, FileLocator_Fixture) {

  setenv(envVariable.c_str(), composedPath.c_str(), 1);
  string file { "MockConfFileForTestingFileLocator_4" };

  bool exception = false;
   try {
     fs::path actualFullPath = searchConfFileInPathVariable(file);
   } catch (const ElementsException & e) {
     //exception = true;
     string exception_str = e.what();
     exception =
         (exception_str.find(
             "found more than once")
             != string::npos);
   }
   BOOST_CHECK(exception);

}

//----------------------------------------------------------------------------------------------------------


BOOST_FIXTURE_TEST_CASE(searchConfFileInPath_1_up_withPrefix, FileLocator_Fixture) {

  setenv(envVariable.c_str(), composedPath.c_str(), 1);
  string file { "MockConfFileForTestingFileLocator_1" };
  string prefix {"ElementsKernel" } ;
  fs::path actualFullPath = searchConfFileInPathVariable(file, prefix);
  fs::path expectedFullPath = fullPath / fs::path { "ElementsKernel" } / fs::path{file}.replace_extension(extension) ;
  cout << actualFullPath << endl;
  cout << expectedFullPath << endl;
  BOOST_CHECK(actualFullPath == expectedFullPath);

}  // Eof searchConfFileInPath_1 up

BOOST_FIXTURE_TEST_CASE(searchConfFileInPath_2_down1_withPrefix, FileLocator_Fixture) {

  setenv(envVariable.c_str(), composedPath.c_str(), 1);
  string file { "MockConfFileForTestingFileLocator_2" };
  string prefix {"ElementsKernel" } ;
   fs::path actualFullPath = searchConfFileInPathVariable(file, prefix);
  fs::path expectedFullPath = fullPath / fs::path { "additionalDirForTesting/ElementsKernel" } / fs::path{file}.replace_extension(extension) ;
  BOOST_CHECK(actualFullPath == expectedFullPath);

}

BOOST_FIXTURE_TEST_CASE(searchConfFileInPath_3_wrongParent_withPrefix, FileLocator_Fixture) {

  setenv(envVariable.c_str(), composedPath.c_str(), 1);
  string file { "MockConfFileForTestingFileLocator_3" };
  string prefix {"ElementsKernel" } ;
   fs::path actualFullPath = searchConfFileInPathVariable(file, prefix);
  BOOST_CHECK(actualFullPath.empty());

}

BOOST_FIXTURE_TEST_CASE(searchConfFileInPath_wrongFile_withPrefix, FileLocator_Fixture) {

  setenv(envVariable.c_str(), composedPath.c_str(), 1);
  string file { "ElementsKernel/Toto" };
  string prefix {"ElementsKernel" } ;
   fs::path actualFullPath = searchConfFileInPathVariable(file, prefix);
   BOOST_CHECK(actualFullPath.empty());

}

BOOST_FIXTURE_TEST_CASE(searchConfFileInPath_twoFilesException_withPrefix, FileLocator_Fixture) {

  setenv(envVariable.c_str(), composedPath.c_str(), 1);
  string file { "MockConfFileForTestingFileLocator_4" };

  bool exception = false;
   try {
     fs::path actualFullPath = searchConfFileInPathVariable(file);
   } catch (const ElementsException & e) {
     //exception = true;
     string exception_str = e.what();
     exception =
         (exception_str.find(
             "found more than once")
             != string::npos);
   }
   BOOST_CHECK(exception);

}

//-----------------------------------------------------------------------------

//BOOST_FIXTURE_TEST_CASE( tokenizePath_test, FileLocatorFix ) {
//
//  char elementsConfPath[] = "ELEMENTS_CONF_PATH=/opt/local/bin:/opt/local/sbin:/usr/local/bin:/usr/bin:/bin";
//  putenv(elementsConfPath);
//  vector<string> pathElements = fileLocator->tokenizeEnvVariable("ELEMENTS_CONF_PATH");
//  for (string & i : pathElements ) {
//    cout << "From test 1 : Path element = " << i << endl;
//  }
//
//  string actualPathElement = pathElements.at(0);
//  string expectedPathElement = "/opt/local/bin";
//  BOOST_CHECK(actualPathElement == expectedPathElement);
//
//  actualPathElement = pathElements.at(3);
//  expectedPathElement = "/usr/bin";
//  BOOST_CHECK(actualPathElement == expectedPathElement);
//
//} // Eof tokenizePath_test

//-----------------------------------------------------------------------------

//BOOST_FIXTURE_TEST_CASE( searchInEnvVariable_test, FileLocatorFix ) {
//
//  std::string envVariable = "ELEMENTS_CONF_PATH";
//  char elementsConfPath[] = "ELEMENTS_CONF_PATH=/opt/local/bin:/Users/binko/Euclid/sw/Alexandria/Integration/tests/conf:/bin";
//  putenv(elementsConfPath);
//  std::string execName = "MockConfFileForTestingFileLocator";
//  string actualFullPath = fileLocator->searchInEnvVariable(envVariable, execName);
//  cout << "From test 2 : File = " << actualFullPath << endl;
//  string expectedFullPath = "/Users/binko/Euclid/sw/Alexandria/Integration/tests/conf/Blabla.conf";
//  BOOST_CHECK(actualFullPath.empty());
//  //BOOST_CHECK(actualFullPath == expectedFullPath);
//
//} // Eof searchInEnvVariable_test

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END ()

//-----------------------------------------------------------------------------
//
// End of the Boost tests
//
//-----------------------------------------------------------------------------
