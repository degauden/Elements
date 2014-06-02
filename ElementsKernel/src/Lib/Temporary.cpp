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

#include "ElementsKernel/ElementsLogging.h"
#include "ElementsKernel/Temporary.h"


using namespace std;
namespace fs = boost::filesystem ;



TempPath::TempPath(const string& motif) : m_motif(motif) {

  if (m_motif != "" ) {
    m_path = fs::temp_directory_path() / fs::unique_path(m_motif);
  } else {
    m_path = fs::temp_directory_path() / fs::unique_path();
  }

}



fs::path TempPath::path() const {
  return m_path ;
}

string TempPath::motif() const {
  return m_motif ;
}

TempDir::TempDir(const string& motif) : TempPath(motif) {

  ElementsLogging logger = ElementsLogging::getLogger("") ;
  logger.debug() << "Creation of the " << path() <<" temporary directory" ;

  fs::create_directory(path()) ;

}

TempDir::~TempDir() {

  ElementsLogging logger = ElementsLogging::getLogger("") ;
  logger.debug() << "Automatic destruction of the " << path() <<" temporary directory" ;

  fs::remove_all(path()) ;

}


TempFile::TempFile(const string& motif) : TempPath(motif) {

  ElementsLogging logger = ElementsLogging::getLogger("") ;
  logger.debug() << "Creation of the " << path() <<" temporary file" ;

  fs::ofstream ofs(path());
  ofs.close();

}

TempFile::~TempFile() {

  ElementsLogging logger = ElementsLogging::getLogger("") ;
  logger.debug() << "Automatic destruction of the " << path() <<" file" ;

  fs::remove(path()) ;

}


