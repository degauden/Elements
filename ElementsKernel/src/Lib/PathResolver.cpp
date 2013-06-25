#include "ElementsKernel/PathResolver.h"
#include "ElementsKernel/System.h"

#ifdef WIN32
// Disable warning
//   C4996: 'std::copy': Function call with parameters that may be unsafe
// Probably coming from Boost classification.
#pragma warning(disable:4996)
#endif

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/filesystem.hpp>

namespace bf = boost::filesystem;
using namespace std;

#ifdef _WIN32
static const char* path_separator = ",;";
#else
static const char* path_separator = ",:";
#endif

//
///////////////////////////////////////////////////////////////////////////
//

namespace Elements {
namespace System {

typedef enum {
  PR_regular_file, PR_directory
} PR_file_type;

typedef enum {
  PR_local, PR_recursive
} PR_search_type;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static bool prFind(const bf::path& file, const string& search_list,
    PR_file_type file_type, PathResolver::SearchType search_type,
    string& result) {

  bool found(false);

  // look for file as specified first

  try {
    if ((file_type == PR_regular_file && is_regular_file(file))
        || (file_type == PR_directory && is_directory(file))) {
      result = bf::system_complete(file).string();
      return true;
    }
  } catch (bf::filesystem_error& /*err*/) {
  }

  // assume that "." is always part of the search path, so check locally first

  try {
    bf::path local = bf::initial_path() / file;
    if ((file_type == PR_regular_file && is_regular_file(local))
        || (file_type == PR_directory && is_directory(local))) {
      result = bf::system_complete(file).string();
      return true;
    }
  } catch (bf::filesystem_error& /*err*/) {
  }

  // iterate through search list
  vector<string> spv;
  split(spv, search_list, boost::is_any_of(path_separator),
      boost::token_compress_on);
  for (vector<string>::const_iterator itr = spv.begin(); itr != spv.end();
      ++itr) {

    bf::path fp = *itr / file;

    try {
      if ((file_type == PR_regular_file && is_regular_file(fp))
          || (file_type == PR_directory && is_directory(fp))) {
        result = bf::system_complete(fp).string();
        return true;
      }
    } catch (bf::filesystem_error& /*err*/) {
    }

    // if recursive searching requested, drill down
    if (search_type == PathResolver::RecursiveSearch
        && is_directory(bf::path(*itr))) {

      bf::recursive_directory_iterator end_itr;
      try {
        for (bf::recursive_directory_iterator ritr(*itr); ritr != end_itr;
            ++ritr) {

          // skip if not a directory
          if (!is_directory(bf::path(*ritr))) {
            continue;
          }

          bf::path fp2 = bf::path(*ritr) / file;
          if ((file_type == PR_regular_file && is_regular_file(fp2))
              || (file_type == PR_directory && is_directory(fp2))) {
            result = bf::system_complete(fp2).string();
            return true;
          }
        }
      } catch (bf::filesystem_error& /*err*/) {
      }
    }

  }

  return found;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

string PathResolver::find_file(const string& logical_file_name,
    const string& search_path, SearchType search_type) {

  string path_list;
  Elements::System::getEnv(search_path, path_list);

  return (find_file_from_list(logical_file_name, path_list, search_type));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

string PathResolver::find_file_from_list(
    const string& logical_file_name, const string& search_list,
    SearchType search_type) {
  string result("");

  bf::path lfn(logical_file_name);

  /* bool found = */
  prFind(lfn, search_list, PR_regular_file, search_type, result);

  // The following functionality was in the original PathResolver, but I believe
  // that it's WRONG. It extracts the filename of the requested item, and searches
  // for that if the preceding search fails. i.e., if you're looking for "B/a.txt",
  // and that fails, it will look for just "a.txt" in the search list.

  // if (! found && lfn.filename() != lfn ) {
  //   result = "";
  //   PR_find (lfn.filename(), search_list, PR_regular_file, search_type, result);
  // }

  return (result);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

string PathResolver::find_directory(const string& logical_file_name,
    const string& search_path, SearchType search_type) {
  string path_list;
  Elements::System::getEnv(search_path, path_list);

  return (find_directory_from_list(logical_file_name, path_list, search_type));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

string PathResolver::find_directory_from_list(
    const string& logical_file_name, const string& search_list,
    SearchType search_type) {
  string result;

  if (!prFind(logical_file_name, search_list, PR_directory, search_type,
      result)) {
    result = "";
  }

  return (result);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

PathResolver::SearchPathStatus PathResolver::check_search_path(
    const string& search_path) {
  string path_list;
  if (!Elements::System::getEnv(search_path, path_list))
    return (EnvironmentVariableUndefined);

  vector<string> spv;
  boost::split(spv, path_list, boost::is_any_of(path_separator),
      boost::token_compress_on);
  vector<string>::iterator itr = spv.begin();

  try {
    for (; itr != spv.end(); ++itr) {
      bf::path pp(*itr);
      if (!is_directory(pp)) {
        return (UnknownDirectory);
      }
    }
  } catch (bf::filesystem_error& /*err*/) {
    return (UnknownDirectory);
  }

  return (Ok);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

string pathResolverFindXMLFile(const string& logical_file_name) {
  return PathResolver::find_file(logical_file_name, "XMLPATH");
}

string pathResolverFindDataFile(const string& logical_file_name) {
  return PathResolver::find_file(logical_file_name, "DATAPATH");
}

}  // Elements::System namespace
}
