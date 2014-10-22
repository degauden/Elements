/**
 * @file Temporary.h
 *
 * @date May 27, 2014
 * @author hubert degaudenzi
 */

#ifndef ELEMENTSKERNEL_TEMPORARY_H_
#define ELEMENTSKERNEL_TEMPORARY_H_


#include <boost/filesystem.hpp>

#include "ElementsKernel/Export.h" // ELEMENTS_API


namespace Elements {

class TempPath {
public:
  TempPath(const std::string& motif) ;
  virtual ~TempPath() {};
  ELEMENTS_API boost::filesystem::path path() const;
  ELEMENTS_API std::string motif() const;
private:
  const std::string m_motif ;
  boost::filesystem::path m_path ;
};


class TempDir : public TempPath {
public:
  ELEMENTS_API TempDir(const std::string& motif="");
  ELEMENTS_API ~TempDir();
};


class TempFile : public TempPath {
public:
  ELEMENTS_API TempFile(const std::string&  motif="") ;
  ELEMENTS_API ~TempFile() ;
};

} // namespace Elements

#endif // ELEMENTSKERNEL_TEMPORARY_H_
