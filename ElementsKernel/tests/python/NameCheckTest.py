'''
Created on Jan 18, 2017

@author: Hubert Degaudenzi

@copyright: 2012-2020 Euclid Science Ground Segment

This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free
Software Foundation; either version 3.0 of the License, or (at your option)
any later version.

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

'''
import unittest
import os

import urllib2
import ssl

from ElementsKernel.NameCheck import getInfo, DEFAULT_TYPE


ELEMENTS_NAMING_DB_URL = os.environ.get("ELEMENTS_NAMING_DB_URL", "")

class NameCheckTestCase(unittest.TestCase):

    def setUp(self):
        unittest.TestCase.setUp(self)
        self.site_exists = True
        if ELEMENTS_NAMING_DB_URL:
            try:
                req = urllib2.Request(ELEMENTS_NAMING_DB_URL + "/NameCheck",
                                      headers={ 'X-Mashape-Key': 'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX' })
                gcontext = ssl.SSLContext(ssl.PROTOCOL_TLSv1)
                urllib2.urlopen(req, context=gcontext)

            except:
                self.site_exists = False
        else:
            self.site_exists = False


    def tearDown(self):
        unittest.TestCase.tearDown(self)

    def testGetDefaultInfo(self):
        if self.site_exists:
            info = getInfo("ElementsKernel", ELEMENTS_NAMING_DB_URL)
            self.assert_(not info["error"])
            self.assert_(info["exists"])
            self.assert_(info["type"] == DEFAULT_TYPE)

    def testGetCMakeInfo(self):
        if self.site_exists:
            info = getInfo("ElementsKernel", ELEMENTS_NAMING_DB_URL, "cmake")
            self.assert_(not info["error"])
            self.assert_(info["exists"])
            self.assert_(info["type"] == "cmake")

    def testGetLibraryInfo(self):
        if self.site_exists:
            info = getInfo("ElementsKernel", ELEMENTS_NAMING_DB_URL, "library")
            self.assert_(not info["error"])
            self.assert_(info["exists"])
            self.assert_(info["type"] == "library")

    def testMissingInfo(self):
        if self.site_exists:
            info = getInfo("Elem__dsdfentskernel", ELEMENTS_NAMING_DB_URL)
            self.assert_(not info["error"])
            self.assert_(not info["exists"])
            self.assert_(info["type"] == "cmake")




if __name__ == '__main__':
    unittest.main()
