/**
 * @file Version.cpp
 *
 * @date Aug 14, 2013
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
 *
 */
#include <string>
#include <vector>
#include <boost/utility.hpp>
#include <boost/algorithm/string.hpp>

#include "ElementsKernel/Version.h"

using std::string;

namespace Elements {

string getVersionFromSvnKeywords(const string& svnUrl, const string& svnId) {

    using std::vector;

     // output to-be-returned version
    string version {};

    // Delimiter to split the URL
    const string delim("/");
    // vector of elements of the URL between pairs of "/"
    vector<string> urlElements {};
    // Build a string vector with the URL elements
    boost::split(urlElements, svnUrl, boost::is_any_of(delim));

    // Loop over all elements of the URL
    for (auto it = urlElements.begin(); it != urlElements.end(); ++it) {
      // If "trunk" is detected...
      if ((*it).find("trunk") != string::npos) {
         // ...return the SVN Id keyword
         version = svnId;
         break;
       }
      // If "tags" id detected ...
      if ((*it).find("tags") != string::npos) {
        // ...built a version from the project name and tags number
        version = *(boost::prior(it)) + " " + *(boost::next(it));
        break;
      }
    }
    return version;
  }

string getVersionString(const unsigned short major, const unsigned short minor, const unsigned short patch) {

  using std::to_string;

  string version {""};

  version += to_string(major);
  version += ".";
  version += to_string(minor);

  if (0 != patch) {
    version += ".";
    version += to_string(patch);
  }

  return version;
}

}  // namespace Elements
