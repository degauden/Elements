/**
 * @file Temporary.cpp
 *
 * @date May 27, 2014
 * @author hubert degaudenzi
 */


#include <string>
#include <iostream>

#include <boost/filesystem.hpp>



#include "ElementsKernel/Temporary.h"


using namespace std;
namespace bf = boost::filesystem ;


TempPath::TempPath(string motif) : m_motif(motif) {

  m_path = bf::temp_directory_path() / bf::unique_path(m_motif);
  this->create() ;

}

void TempPath::create() {

  cout << "TmpPath Creation" << endl ;

}

TempPath::~TempPath() {

  this->destroy() ;

}

bf::path TempPath::path() const {
  return m_path ;
}

string TempPath::motif() const {
  return m_motif ;
}



void TempDir::create() {

  cout << "TmpDir Creation" << endl ;
}

void TempDir::destroy() {

}


void TempFile::create() {

  cout << "TmpFile Creation" << endl ;

}

void TempFile::destroy() {

}
