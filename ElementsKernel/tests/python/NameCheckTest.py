#
# Copyright (C) 2012-2020 Euclid Science Ground Segment
# 
# This library is free software; you can redistribute it and/or modify it under
# the terms of the GNU Lesser General Public License as published by the Free
# Software Foundation; either version 3.0 of the License, or (at your option)
# any later version.
# 
# This library is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
# details.
# 
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
#


'''
:date: Jan 18, 2017

:author: Hubert Degaudenzi

'''

import unittest
import os

from ElementsKernel.NameCheck import getInfo, DEFAULT_TYPE, checkDataBaseUrl


ELEMENTS_NAMING_DB_URL = os.environ.get("ELEMENTS_NAMING_DB_URL", "")

class NameCheckTestCase(unittest.TestCase):

    def setUp(self):
        unittest.TestCase.setUp(self)
        self.site_exists = checkDataBaseUrl(ELEMENTS_NAMING_DB_URL)

    def tearDown(self):
        unittest.TestCase.tearDown(self)

    def testGetDefaultInfo(self):
        if self.site_exists:
            info = getInfo("ElementsKernel", ELEMENTS_NAMING_DB_URL)
            self.assertFalse(info["error"])
            self.assertTrue(info["exists"])
            self.assertEqual(info["type"], DEFAULT_TYPE)

    def testGetCMakeInfo(self):
        if self.site_exists:
            info = getInfo("ElementsKernel", ELEMENTS_NAMING_DB_URL, "cmake")
            self.assertFalse(info["error"])
            self.assertTrue(info["exists"])
            self.assertEqual(info["type"], "cmake")

    def testGetLibraryInfo(self):
        if self.site_exists:
            info = getInfo("ElementsKernel", ELEMENTS_NAMING_DB_URL, "library")
            self.assertFalse(info["error"])
            self.assertTrue(info["exists"])
            self.assertEqual(info["type"], "library")

    def testMissingInfo(self):
        if self.site_exists:
            info = getInfo("Elem__dsdfentskernel", ELEMENTS_NAMING_DB_URL)
            self.assertFalse(info["error"])
            self.assertFalse(info["exists"])
            self.assertEqual(info["type"], "cmake")

if __name__ == '__main__':
    unittest.main()
