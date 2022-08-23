/**
 * @file Version_test.cpp
 *
 * Created on: Aug 15, 2013
 *     Author: Pierre Dubath
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

#include "ElementsKernel/Version.h"

#include <string>  // for std::string

#include <boost/test/unit_test.hpp>

using std::string;

namespace Elements {
//-----------------------------------------------------------------------------

// input string to test the generation of an appropriate version in case of a trunk or tags
const string SVN_ID{"SVN $Id: Program.cpp 3124 2013-08-14 12:20:26Z pdubath $"};
const string SVN_URL_TRUNK{"SVN $HeadURL: "
                           "http://euclid.esac.esa.int/svn/EC/SGS/SDC/CH/Projects/Elements/trunk/ElementsExamples/src/"
                           "program/Program.cpp $"};
const string SVN_URL_TAGS{"SVN $HeadURL: "
                          "http://euclid.esac.esa.int/svn/EC/SGS/SDC/CH/Projects/Elements/tags/1.2/ElementsExamples/"
                          "src/program/ElementsProgram.cpp $"};

// expected result
const string EXPECTED_TRUNK_VERSION{"SVN $Id: Program.cpp 3124 2013-08-14 12:20:26Z pdubath $"};
const string EXPECTED_TAGS_VERSION{"Elements 1.2"};

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE(Version_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(getVersionFromSvnKeywords_trunk) {
  BOOST_CHECK(getVersionFromSvnKeywords(SVN_URL_TRUNK, SVN_ID).find(EXPECTED_TRUNK_VERSION) != string::npos);
}

BOOST_AUTO_TEST_CASE(getVersionFromSvnKeywords_tags) {
  BOOST_CHECK(getVersionFromSvnKeywords(SVN_URL_TAGS, SVN_ID).find(EXPECTED_TAGS_VERSION) != string::npos);
}

BOOST_AUTO_TEST_CASE(getVersionFromString_test) {
  BOOST_CHECK(getVersionString(1, 2) == string("1.2"));
  BOOST_CHECK(getVersionString(3, 2, 1) == string("3.2.1"));
  BOOST_CHECK(getVersionString(4, 7, 0) == string("4.7"));
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace Elements
