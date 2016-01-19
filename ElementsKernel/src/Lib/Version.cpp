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

} // Elements namespace
