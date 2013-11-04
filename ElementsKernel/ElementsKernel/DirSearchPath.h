#ifndef JOBOPTIONS_DIRSEARCHPATH_H
#define JOBOPTIONS_DIRSEARCHPATH_H
/** @file DirSearchPath.h
 * @brief search for files in a list of directories
 * @author Paolo Calafiura <pcalafiura@lbl.gov> - ATLAS Collaboration
 * $Id: DirSearchPath.h,v 1.2 2007/10/16 15:37:25 marcocle Exp $
 */
#include <list>
#include <functional>  /* binary_function */
#include <string>
#include "boost/filesystem/path.hpp"
#include "boost/filesystem/exception.hpp" /*filesystem_error*/
#include "ElementsKernel/Export.h" /* ELEMENTS_API */
/** @class DirSearchPath
 * @brief search for files in a list of directories
 * @author Paolo Calafiura <pcalafiura@lbl.gov> - ATLAS Collaboration
 * $Id: DirSearchPath.h,v 1.2 2007/10/16 15:37:25 marcocle Exp $
 */
class ELEMENTS_API DirSearchPath {
public:
  typedef boost::filesystem::path path;

  /// \name constructors
  //@{
  DirSearchPath() {
    addCWD();
  }
  /// \throws boost::filesystem::filesystem_error
#ifdef _WIN32
  DirSearchPath(const std::string& stringifiedPath, const char* separator=",;", const bool add_cwd=false );
#else
  DirSearchPath(const std::string& stringifiedPath, const char* separator = ",:", const bool add_cwd=false );
#endif
  //@}

  /// \name modifiers
  //@{
  //  bool add(const std::string& dirName); ///< \throws filesystem_error
  bool add(const path& dir);            ///< \throws filesystem_error
  bool addCWD();         ///< add current work dir (*nix pwd) to path
  //@}

  /// \name accessors
  //@{
  ///returns a flag if fileName found in search path, and sets ref to
  ///fully qualified file name (in native form)
  bool find(const std::string& fileName, std::string& fullFileName) const;
  ///returns a flag if file found in search path. Sets ref to completed path
  bool find(const path& file, path& fileFound) const;
  ///returns lists of files found in search path.
  std::list<path> find_all(const path& file) const;
  //@}

  /// \name helpers
  //@{
  static bool existsDir(const std::string& dirName); ///< check dirName is valid
  static bool existsDir(const path& dir); ///< check dir path is valid
  //@}

private:
  //
  /// @class eqPath compare paths name
  struct eqPath: public std::unary_function<const path&, bool> {
    eqPath(const path& ref) :
        m_ref(ref) {
    }
    bool operator()(const path& p) const {
      return p.string() == m_ref.string();
    }
  private:
    path m_ref;
  };
  // @class lessPath order paths by (system-independent) name
  //struct lessPath : public std::binary_function<const path&,const path&,bool> {
  // bool operator() (const path& lhs, const path& rhs) const {
  //   return lhs.string() < rhs.string();
  // }
  //};
  ///

  //  typedef std::set<path, lessPath> PathSet; ///<a set ordered by path name
  //  PathSet m_dirs;   ///<the dir container
  //
  std::list<path> m_dirs{};   ///<the dir container
};
#endif // JOBOPTIONS_DIRSEARCHPATH_H
