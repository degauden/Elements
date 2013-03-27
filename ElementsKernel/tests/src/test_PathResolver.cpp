#include "ElementsKernel/PathResolver.h"
#include "ElementsKernel/System.h"

#include <string>
#include <iostream>
#include <stdlib.h>
#include <boost/filesystem.hpp>

using namespace Elements::System;
namespace bf = boost::filesystem;

int main() {
  int err(0);
  std::cout << "*** PathResolver_test starts ***" << std::endl;

  std::string local = bf::initial_path().string();
  std::cout << "initial path: " << local << std::endl;

  std::string ref1("../tests/PathResolver/A/a.txt");
  std::string ref2("../tests/PathResolver/B/a.txt");
  std::string ref3("../tests/PathResolver/C/c.txt");

#ifdef _WIN32
  std::string ref4("C:/WINNT/explorer.exe");
  setEnv("DATAPATH","..\\tests\\PathResolver;..\\tests\\PathResolver\\A;..\\tests\\PathResolver\\B",1);
#else
#if defined(__APPLE__)
  std::string ref4("/usr/bin/true");
#else
  std::string ref4("/bin/true");
#endif
  setEnv("DATAPATH",
      "../tests/PathResolver:../tests/PathResolver/A:../tests/PathResolver/B",
      1);
#endif

  std::cout << "DATAPATH: " << getEnv("DATAPATH") << std::endl;

  std::string filename = "a.txt";
  std::string fileLocation("");

  fileLocation = PathResolver::find_file(filename, "DATAPATH");
  std::cout << "search for: " << filename << std::endl;
  std::cout << "fileLocation: " << fileLocation << std::endl;
  if (fileLocation == "") {
    std::cout << "  => ERROR: file not found." << std::endl;
    err++;
  } else if (fileLocation.find(local) != std::string::npos) {
    fileLocation.replace(fileLocation.find(local), local.length() + 1, "");
    std::cout << "  -> \"" << fileLocation << "\"" << std::endl;
    if (fileLocation != ref1) {
      std::cout << "  => ERROR: should have been \"" << ref1 << "\""
          << std::endl;
      err++;
    }
  } else {
    std::cout << "  => ERROR: could not remove local path from fileLocation"
        << std::endl;
    err++;
  }

  fileLocation = "";

  filename = "B/a.txt";
  fileLocation = PathResolver::find_file(filename, "DATAPATH");

  std::cout << "search for: " << filename << std::endl;
  std::cout << "fileLocation: " << fileLocation << std::endl;
  if (fileLocation == "") {
    std::cout << "  => ERROR: file not found." << std::endl;
    err++;
  } else if (fileLocation.find(local) != std::string::npos) {
    fileLocation.replace(fileLocation.find(local), local.length() + 1, "");
    std::cout << "  -> \"" << fileLocation << "\"" << std::endl;
    if (fileLocation != ref2) {
      std::cout << "  => ERROR: should have been \"" << ref2 << "\""
          << std::endl;
      err++;
    }
  } else {
    std::cout << "  => ERROR: could not remove local path from fileLocation"
        << std::endl;
    err++;
  }
  fileLocation = "";

  filename = "c.txt";
  fileLocation = PathResolver::find_file(filename, "DATAPATH",
      PathResolver::RecursiveSearch);

  std::cout << "recursive search for: " << filename << std::endl;
  std::cout << "fileLocation:" << fileLocation << std::endl;
  if (fileLocation == "") {
    std::cout << "  => ERROR: file not found." << std::endl;
    err++;
  } else if (fileLocation.find(local) != std::string::npos) {
    fileLocation.replace(fileLocation.find(local), local.length() + 1, "");
    std::cout << "  -> \"" << fileLocation << "\"" << std::endl;
    if (fileLocation != ref3) {
      std::cout << "  => ERROR: should have been \"" << ref3 << "\""
          << std::endl;
      err++;
    }
  } else {
    std::cout << "  => ERROR: could not remove local path from fileLocation"
        << std::endl;
    err++;
  }
  fileLocation = "";

  filename = ref4;
  fileLocation = PathResolver::find_file(filename, "DATAPATH");

  std::cout << "search for: " << filename << std::endl;
  std::cout << "fileLocation: " << fileLocation << std::endl;
  if (fileLocation == "") {
    std::cout << "  => ERROR: file not found." << std::endl;
    err++;
  } else if (fileLocation != filename) {
    std::cout << "  => ERROR: should have been \"" << ref4 << "\"  was \""
        << fileLocation << "\"" << std::endl;
    err++;
  }

  std::cout << "*** PathResolver_test ends (" << (err == 0 ? "OK" : "ERROR")
      << ") ***" << std::endl;

  return (err);
}
