#include "ElementsKernel/PathResolver.h"
#include "ElementsKernel/System.h"

#include <string>
#include <iostream>
#include <cstdlib>
#include <boost/filesystem.hpp>

using namespace std;
using namespace Elements::System;
namespace fs = boost::filesystem;

int main() {
  int err(0);
  cout << "*** PathResolver_test starts ***" << endl;

  string local = fs::initial_path().string();
  cout << "initial path: " << local << endl;

  string ref1("../tests/PathResolver/A/a.txt");
  string ref2("../tests/PathResolver/B/a.txt");
  string ref3("../tests/PathResolver/C/c.txt");

#ifdef _WIN32
  string ref4("C:/WINNT/explorer.exe");
  setEnv("DATAPATH","..\\tests\\PathResolver;..\\tests\\PathResolver\\A;..\\tests\\PathResolver\\B",1);
#else
#if defined(__APPLE__)
  string ref4("/usr/bin/true");
#else
  string ref4("/bin/true");
#endif
  setEnv("DATAPATH", "../tests/PathResolver:../tests/PathResolver/A:../tests/PathResolver/B", 1);
#endif

  cout << "DATAPATH: " << getEnv("DATAPATH") << endl;

  string filename = "a.txt";
  string fileLocation("");

  fileLocation = PathResolver::find_file(filename, "DATAPATH");
  cout << "search for: " << filename << endl;
  cout << "fileLocation: " << fileLocation << endl;
  if (fileLocation == "") {
    cout << "  => ERROR: file not found." << endl;
    err++;
  } else if (fileLocation.find(local) != string::npos) {
    fileLocation.replace(fileLocation.find(local), local.length() + 1, "");
    cout << "  -> \"" << fileLocation << "\"" << endl;
    if (fileLocation != ref1) {
      cout << "  => ERROR: should have been \"" << ref1 << "\"" << endl;
      err++;
    }
  } else {
    cout << "  => ERROR: could not remove local path from fileLocation" << endl;
    err++;
  }

  filename = "B/a.txt";
  fileLocation = PathResolver::find_file(filename, "DATAPATH");

  cout << "search for: " << filename << endl;
  cout << "fileLocation: " << fileLocation << endl;
  if (fileLocation == "") {
    cout << "  => ERROR: file not found." << endl;
    err++;
  } else if (fileLocation.find(local) != string::npos) {
    fileLocation.replace(fileLocation.find(local), local.length() + 1, "");
    cout << "  -> \"" << fileLocation << "\"" << endl;
    if (fileLocation != ref2) {
      cout << "  => ERROR: should have been \"" << ref2 << "\"" << endl;
      err++;
    }
  } else {
    cout << "  => ERROR: could not remove local path from fileLocation" << endl;
    err++;
  }

  filename = "c.txt";
  fileLocation = PathResolver::find_file(filename, "DATAPATH", PathResolver::RecursiveSearch);

  cout << "recursive search for: " << filename << endl;
  cout << "fileLocation:" << fileLocation << endl;
  if (fileLocation == "") {
    cout << "  => ERROR: file not found." << endl;
    err++;
  } else if (fileLocation.find(local) != string::npos) {
    fileLocation.replace(fileLocation.find(local), local.length() + 1, "");
    cout << "  -> \"" << fileLocation << "\"" << endl;
    if (fileLocation != ref3) {
      cout << "  => ERROR: should have been \"" << ref3 << "\"" << endl;
      err++;
    }
  } else {
    cout << "  => ERROR: could not remove local path from fileLocation" << endl;
    err++;
  }

  filename = ref4;
  fileLocation = PathResolver::find_file(filename, "DATAPATH");

  cout << "search for: " << filename << endl;
  cout << "fileLocation: " << fileLocation << endl;
  if (fileLocation == "") {
    cout << "  => ERROR: file not found." << endl;
    err++;
  } else if (fileLocation != filename) {
    cout << "  => ERROR: should have been \"" << ref4 << "\"  was \"" << fileLocation << "\"" << endl;
    err++;
  }

  cout << "*** PathResolver_test ends (" << (err == 0 ? "OK" : "ERROR") << ") ***" << endl;

  return (err);
}
