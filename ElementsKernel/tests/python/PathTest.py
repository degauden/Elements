'''
Created on Apr 29, 2016

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

from ElementsKernel.Temporary import TempDir
from ElementsKernel.Path import joinPath, multiPathAppend

class PathTest(unittest.TestCase):


    def setUp(self):
        unittest.TestCase.setUp(self)
        self._tmpdir_1 = TempDir(suffix="tempdir")
        self._tmpdir_2 = TempDir(suffix="tempdir")


    def tearDown(self):
        unittest.TestCase.tearDown(self)
        del self._tmpdir_1
        del self._tmpdir_2

    def testJoinPath(self):

        path_list = ["/toto", "titi", "./tutu"]
        self.assert_(joinPath(path_list) == "/toto:titi:./tutu")

        path_list2 = ["", "/toto", "titi", "./tutu"]
        self.assert_(joinPath(path_list2) == ":/toto:titi:./tutu")

        path_list3 = ["/toto", "titi", "./tutu", ""]
        self.assert_(joinPath(path_list3) == "/toto:titi:./tutu:")

    def testMultiPathAppend(self):

        locations = ["loc1", "/loc2", "./loc3"]
        suffixes = ["bin", "scripts"]

        ref_paths = ["loc1/bin", "loc1/scripts",
                     "/loc2/bin", "/loc2/scripts",
                     "./loc3/bin", "./loc3/scripts"]

        self.assert_(multiPathAppend(locations, suffixes) == ref_paths)

if __name__ == "__main__":
    # import sys;sys.argv = ['', 'Test.testName']
    unittest.main()
