/**
 * @file Temporary.h
 *
 * @date May 27, 2014
 * @author hubert degaudenzi
 */

#ifndef TEMPORARY_H_
#define TEMPORARY_H_

#include <string>

#include <boost/filesystem.hpp>

#include "ElementsKernel/Export.h" // ELEMENTS_API

class TempPath {
public:
  TempPath(const std::string& motif) ;
  virtual ~TempPath() {};
  boost::filesystem::path path() const;
  std::string motif() const;
private:
  const std::string m_motif ;
  boost::filesystem::path m_path ;

};


class TempDir : public TempPath {
public:
  TempDir(const std::string& motif="");
  ~TempDir();

};


class TempFile : public TempPath {
public:
  TempFile(const std::string&  motif="") ;
  ~TempFile() ;

};


#endif // TEMPORARY_H_ 
