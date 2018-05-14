/**
 * @file ElementsKernel/Version.h
 * @date Jun 20, 2013
 * @author Pierre Dubath
 *
 * @copyright 2012-2020 Euclid Science Ground Segment
 *
 * This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General
 * Public License as published by the Free Software Foundation; either version 3.0 of the License, or (at your option)
 * any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @addtogroup ElementsKernel ElementsKernel
 * @{
 */

#ifndef ELEMENTSKERNEL_ELEMENTSKERNEL_VERSION_H_
#define ELEMENTSKERNEL_ELEMENTSKERNEL_VERSION_H_

#include <string>                   // for string
#include <cstdint>

#include "ElementsKernel/Export.h"  // ELEMENTS_API

constexpr std::uint_least64_t CALC_PROJECT_VERSION(const std::uint_least64_t maj,
                                                   const std::uint_least64_t min,
                                                   const std::uint_least64_t pat) {
  return (((maj) << 32) + ((min) << 16) + (pat));
}

namespace Elements {

/**
 * @brief
 *    Function returning a version string extracted from SVN keywords
 * @details
 *    ElementsExample/ElementsProgramExample.cpp shows how to use this method to implement
 *    a getVersion in a prgram
 * @ingroup ElementsKernel
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
 * @ingroup ElementsKernel
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
                                          const unsigned short patch = 0);

}  // namespace Elements

#endif  // ELEMENTSKERNEL_ELEMENTSKERNEL_VERSION_H_

/**@}*/
