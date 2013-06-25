#ifndef ELEMENTSKERNEL_PATHRESOLVER_H
#define ELEMENTSKERNEL_PATHRESOLVER_H

#include <string>
#include "ElementsKernel/Kernel.h"

namespace Elements {
namespace System {

class ELEMENTS_API PathResolver

{
public:
  typedef enum {
    Ok, EnvironmentVariableUndefined, UnknownDirectory
  } SearchPathStatus;

  typedef enum {
    LocalSearch, RecursiveSearch
  } SearchType;

  /**

   @arg @c logical_file_name the name of the file to locate in the search path
   @arg @c search_path the name of a path-like environment variable
   @arg @c search_type characterizes the type of search. Can be either @c LocalSearch or @c RecursiveSearch

   @return the physical name of the located file or empty string if not found

   */
  static std::string find_file(const std::string& logical_file_name,
      const std::string& search_path, SearchType search_type = LocalSearch);

  /**

   @arg @c logical_file_name the name of the file to locate in the search path
   @arg @c search_list the prioritized list of possible locations separated by the usual path separator
   @arg @c search_type characterizes the type of search. Can be either @c LocalSearch or @c RecursiveSearch

   @return the physical name of the located file or empty string if not found

   */
  static std::string find_file_from_list(const std::string& logical_file_name,
      const std::string& search_list, SearchType search_type = LocalSearch);

  /**

   @arg @c logical_file_name the name of the directory to locate in the search path
   @arg @c search_path the name of a path-like environment variable
   @arg @c search_type characterizes the type of search. Can be either LocalSearch or RecursiveSearch

   @return the physical name of the located directory or empty string if not found

   */
  static std::string find_directory(const std::string& logical_file_name,
      const std::string& search_path, SearchType search_type = LocalSearch);

  /**

   @arg @c logical_file_name the name of the directory to locate in the search path
   @arg @c search_list the prioritized list of possible locations separated by the usual path separator
   @arg @c search_type characterizes the type of search. Can be either LocalSearch or RecursiveSearch

   @return the physical name of the located directory or empty string if not found

   */
  static std::string find_directory_from_list(
      const std::string& logical_file_name, const std::string& search_list,
      SearchType search_type = LocalSearch);

  /**

   @arg @c search_path the name of a path-like environment variable

   @return the result of the verification. Can be one of @c Ok, @c EnvironmentVariableUndefined, @c UnknownDirectory

   */
  static SearchPathStatus check_search_path(const std::string& search_path);

};
ELEMENTS_API std::string pathResolverFindXMLFile(
    const std::string& logical_file_name);
ELEMENTS_API std::string pathResolverFindDataFile(
    const std::string& logical_file_name);

}
}
#endif
