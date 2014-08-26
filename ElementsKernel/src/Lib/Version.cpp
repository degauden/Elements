/**
 * @file Version.cpp
 *
 * Created on: Aug 14, 2013
 *     Author: Pierre Dubath
 */
#include <string>
#include <vector>
#include <boost/utility.hpp>
#include <boost/algorithm/string.hpp>

#include "ElementsKernel/Version.h"

using namespace std;

namespace Elements {

std::string getVersionFromSvnKeywords(std::string svnUrl, std::string svnId) {

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

} // Elements namespace
