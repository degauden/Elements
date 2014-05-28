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


class TempPath {
public:
  TempPath(std::string motif) ;
  virtual void create();
  virtual void destroy() {};
  virtual ~TempPath() ;
  boost::filesystem::path path() const;
  std::string motif() const;
private:
  std::string m_motif ;
  boost::filesystem::path m_path ;

};


class TempDir : public TempPath {
public:
  TempDir(std::string motif) : TempPath(motif) {} ;
  void create() override;
  void destroy() override;

};


class TempFile : public TempPath {
public:
  TempFile(std::string motif) : TempPath(motif) {} ;
  void create() override;
  void destroy() override;

};





#endif // TEMPORARY_H_ 
