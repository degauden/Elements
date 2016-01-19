/**
 * @file Version.h
 * @date Jun 20, 2013
 * @author Pierre Dubath - The Euclid Consortium
 */

#ifndef VERSION_H_
#define VERSION_H_

#include "ElementsKernel/Export.h" // ELEMENTS_API

namespace Elements {

/**
 * @brief
 *    Function returning a version string extracted from SVN keywords
 * @details
 *    ElementsExample/ElementsProgramExample.cpp shows how to use this method to implement
 *    a getVersion in a prgram
 * @param svnUrl
 *    This is a string that can be filled with the HeadURL svn keywords. Writing
 *       \#define SVN_URL "SVN $HeadURL$
 *    in a file.
 * @param svnId
 *     This is a string that can be filled with the Id svn keywords. Writing
 *       \#define SVN_ID "SVN $Id$
 *    in a file.
 *
 *    the svn keywords will be expanded upon the next commit if the svn
 *    client is instructed to do this with
 *       svn propset svn:keywords 'Id Revision HeadURL' ElementsProgramExample.cpp
 *
 * @return
 */
ELEMENTS_API std::string getVersionFromSvnKeywords(const std::string& svnUrl, const std::string& svnId);

/**
 * @brief
 *    Function converting the version numbers into a string
 * @details
 *    This function return either 2 numbers separated with a "." if the patch number is zero.
 *    Or 3 numbers separated with a "." if the patch is not zero.
 * @param major
 *    Major version number
 * @param minor
 *    Minor version number
 * @param patch
 *    Patch version number
 * @return
 *    The formatted string with the version numbers
 */
ELEMENTS_API std::string getVersionString(const unsigned short major,
                                          const unsigned short minor,
                                          const unsigned short patch=0);

} // Elements namespace

#endif /* VERSION_H_ */
