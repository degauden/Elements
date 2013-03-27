/** unit test for DirSearchPath class
 * ---------------------------------------------------------------------
 * @author Paolo Calafiura <pcalafiura@lbl.gov> - ATLAS Collaboration
 */

// $Id: DirSearchPath_test.cpp,v 1.2 2007/10/22 18:53:46 marcocle Exp $
//<<<<<< INCLUDES                                                       >>>>>>
#include <cassert>
#include <iostream>
#ifdef __ICC
// disable icc warning #279: controlling expression is constant
// ... a lot of noise produced by the boost/filesystem/operations.hpp
#pragma warning(disable:279)
#endif
#include "boost/filesystem/operations.hpp"
#include "ElementsKernel/DirSearchPath.h"

using namespace std;
using namespace boost::filesystem;

int main() {
  cout << "*** DirSearchPath_test Starts ***" << endl;
  DirSearchPath bad("foo:fo:  : *");
  try {
    //make sure tmp is there
    path tmp("tmp");
    if (!exists(tmp))
      create_directory(tmp);

    //create a test dir tree under tmp
    path testRoot("tmp/DirSearchPath_test");
    if (exists(testRoot) && !testRoot.empty())
      remove_all(testRoot);
    if (!exists(testRoot))
      create_directory(testRoot);
    path testSub1(testRoot / path("sub1"));
    create_directory(testSub1);
    path testSub2(testRoot / path("sub2"));
    create_directory(testSub2);
    path testSub2Sub3(testSub2 / path("sub3"));
    create_directory(testSub2Sub3);

    //add work areas to path
#ifndef _WIN32
    DirSearchPath searchPath(" :tmp: bla");
#else
    DirSearchPath searchPath(" ;tmp; bla");
#endif
    //now look for something
    string fullFileName;
    assert(searchPath.find("DirSearchPath_test", fullFileName));
    assert(searchPath.add("tmp/DirSearchPath_test"));
    assert(searchPath.find("sub1", fullFileName));
    assert(searchPath.find("sub2", fullFileName));
    assert(!searchPath.find("sub3", fullFileName));
    assert(searchPath.find("sub2/sub3", fullFileName));
    assert(searchPath.add(testSub2));
    assert(searchPath.find("sub3", fullFileName));
    if (!testRoot.empty())
      remove_all(testRoot);
  } catch (const filesystem_error& err) {
    cerr << err.what() << endl;
#ifndef NDEBUG
    const bool SHOULDNEVERGETHERE(false);
    assert(SHOULDNEVERGETHERE);
#endif
  }
  cout << "*** DirSearchPath_test OK ***" << endl;
  return 0;
}
