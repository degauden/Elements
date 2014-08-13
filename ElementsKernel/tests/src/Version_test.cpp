/**
 * @file Version_test.cpp
 *
 * Created on: Aug 15, 2013Jun 20, 2013
 *     Author: Pierre Dubath
 */

#include <boost/test/unit_test.hpp>
#include "ElementsKernel/Version.h"

using namespace std;

//-----------------------------------------------------------------------------


// input string to test the generation of an appropriate version in case of a trunk or tags
string SVN_ID {"SVN $Id: ElementsProgramExample.cpp 3124 2013-08-14 12:20:26Z pdubath $"};
string SVN_URL_TRUNK {"SVN $HeadURL: http://euclid.esac.esa.int/svn/EC/SGS/SDC/CH/Projects/Elements/trunk/ElementsExamples/src/Program/ElementsProgramExample.cpp $"};
string SVN_URL_TAGS {"SVN $HeadURL: http://euclid.esac.esa.int/svn/EC/SGS/SDC/CH/Projects/Elements/tags/1.2/ElementsExamples/src/Program/ElementsProgramExample.cpp $"};

// expected result
string expectedTrunkVersion {"SVN $Id: ElementsProgramExample.cpp 3124 2013-08-14 12:20:26Z pdubath $"};
string expectedTagsVersion {"Elements 1.2"};

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE (Version_test)

//-----------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(getVersionFromSvnKeywords_trunk) {
  BOOST_CHECK(getVersionFromSvnKeywords(SVN_URL_TRUNK, SVN_ID).find(expectedTrunkVersion) != string::npos );
}

BOOST_AUTO_TEST_CASE(getVersionFromSvnKeywords_tags) {
  BOOST_CHECK(getVersionFromSvnKeywords(SVN_URL_TAGS, SVN_ID).find(expectedTagsVersion) != string::npos );
}

BOOST_AUTO_TEST_SUITE_END ()
