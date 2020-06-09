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
:date: Apr 29, 2016

:author: Hubert Degaudenzi

'''
import unittest
import subprocess

from ElementsKernel.Temporary import TempDir
from ElementsKernel.Path import joinPath, multiPathAppend, getLocationsFromEnv
from ElementsKernel.Path import getLocations
from ElementsKernel.Path import which
from ElementsKernel.Path import getTargetPath
from ElementsKernel.Path import removeDuplicates


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
        self.assertEqual(joinPath(path_list), "/toto:titi:./tutu")

        path_list2 = ["", "/toto", "titi", "./tutu"]
        self.assertEqual(joinPath(path_list2), ":/toto:titi:./tutu")

        path_list3 = ["/toto", "titi", "./tutu", ""]
        self.assertEqual(joinPath(path_list3), "/toto:titi:./tutu:")

    def testMultiPathAppend(self):

        locations = ["loc1", "/loc2", "./loc3"]
        suffixes = ["bin", "scripts"]

        ref_paths = ["loc1/bin", "loc1/scripts",
                     "/loc2/bin", "/loc2/scripts",
                     "./loc3/bin", "./loc3/scripts"]

        self.assertEqual(multiPathAppend(locations, suffixes), ref_paths)
        
    def testGetLoctions(self):

        # by construction the PATH variable is never empty
        self.assertNotEqual(getLocations(), [])

    def testGetLocationsFromEnv(self):

        self.assertEqual(getLocationsFromEnv("NonExistingEnvVar"), [])
        self.assertNotEqual(getLocationsFromEnv("PATH"), [])

        tmp_list = getLocationsFromEnv("PATH", exist_only=True)
        self.assertNotEqual(len(tmp_list), 0)

    def testWhich(self):
        sys_ls = subprocess.check_output(["which", "ls"]).strip()
        self.assertEqual(sys_ls, which(sys_ls))

    def testGetTargetPath(self):
        
        file_name = "toto"
        target_dir = "/blab/blo"
        self.assertEqual(getTargetPath(file_name, target_dir), "/blab/blo/toto")
        
        file_name = "tata/toto"
        target_dir = "/blab/blo"
        self.assertEqual(getTargetPath(file_name, target_dir), "/blab/blo/toto")

        file_name = "tata/toto"
        target_dir = "/blab/blo"
        self.assertEqual(getTargetPath(file_name, target_dir, use_stem=True), "/blab/blo/tata/toto")

        file_name = "tata/toto"
        target_dir = "/blab/blo"
        target_name = "tutu"
        self.assertEqual(getTargetPath(file_name, target_dir, target_name), "/blab/blo/tutu")

        file_name = "tata/toto"
        target_dir = "/blab/blo"
        target_name = "foo/tutu"
        self.assertEqual(getTargetPath(file_name, target_dir, target_name), "/blab/blo/foo/tutu")


    def testRemoveDuplicates(self):
        locations = ["/usr/bin", "/usr/local/bin",
                     "/usr/bin", "/opt/bin", "/opt/local/bin",
                     "/usr/bin", "/usr/local/bin"]

        unique_locations = ["/usr/bin", "/usr/local/bin",
                            "/opt/bin", "/opt/local/bin"]
        self.assertEqual(removeDuplicates(locations), unique_locations)

if __name__ == "__main__":
    # import sys;sys.argv = ['', 'Test.testName']
    unittest.main()
