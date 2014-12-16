/**
 * @file Temporary.cpp
 *
 * @date May 27, 2014
 * @author hubert degaudenzi
 */


#include <string>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include "ElementsKernel/Logging.h"
#include "ElementsKernel/Temporary.h"


using namespace std;
namespace fs = boost::filesystem ;

namespace Elements {

TempPath::TempPath(const string& motif) : m_motif(motif) {

  if (m_motif != "" ) {
    m_path = fs::temp_directory_path() / fs::unique_path(m_motif);
  } else {
    m_path = fs::temp_directory_path() / fs::unique_path();
  }

}

TempPath::~TempPath() {}

fs::path TempPath::path() const {
  return m_path ;
}

string TempPath::motif() const {
  return m_motif ;
}

TempDir::TempDir(const string& motif) : TempPath(motif) {

  Logging logger = Logging::getLogger() ;
  logger.debug() << "Creation of the " << path() <<" temporary directory" ;

  fs::create_directory(path()) ;

}

TempDir::~TempDir() {

  Logging logger = Logging::getLogger() ;
  logger.debug() << "Automatic destruction of the " << path() <<" temporary directory" ;

  fs::remove_all(path()) ;

}


TempFile::TempFile(const string& motif) : TempPath(motif) {

  Logging logger = Logging::getLogger() ;
  logger.debug() << "Creation of the " << path() <<" temporary file" ;

  fs::ofstream ofs(path());
  ofs.close();

}

TempFile::~TempFile() {

  Logging logger = Logging::getLogger() ;
  logger.debug() << "Automatic destruction of the " << path() <<" file" ;

  fs::remove(path()) ;

}

} // namespace Elements
