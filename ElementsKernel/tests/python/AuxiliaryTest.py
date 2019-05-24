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

import os
import unittest
import subprocess

from ElementsKernel.Temporary import TempDir, TempEnv
from ElementsKernel.Auxiliary import configure, getAuxiliaryPath


class AuxiliaryTest(unittest.TestCase):
    
    class TestFile(object):
        def __init__(self, path, content=""):
            self.setPath(path)
            self.setContent(content)
        def setPath(self, path):
            self._path = path
        def path(self):
            return self._path
        def setContent(self, content):
            self._content = content
            if self._content:
                parent_path = os.path.dirname(self._path)
                if not os.path.exists(parent_path):
                    os.makedirs(parent_path)
                with open(self._path, "w") as f:
                    f.write(self._content)
             
        def content(self):
            return self._content

    def setUp(self):
        unittest.TestCase.setUp(self)
        self._tmpdir = TempDir(suffix="aux_tempdir")
        self._tmpenv = TempEnv()
        self._exiting_dir = os.path.join(self._tmpdir.path(), "ThisDir")
        os.mkdir(self._exiting_dir)
        self._tmpenv["ELEMENTS_AUX_PATH"] = self._tmpdir.path()
        self._test_files = []
        self._test_files.append(AuxiliaryTest.TestFile(os.path.join(self._tmpdir.path(),
                                                                    "file1"),
                                                       "That content no replacement"))
        self._test_files.append(AuxiliaryTest.TestFile(os.path.join(self._tmpdir.path(),
                                                                    "tata", "file1"),
                                                       "This content no replacement"))

        self._test_files.append(AuxiliaryTest.TestFile(os.path.join(self._tmpdir.path(),
                                                                    "tata", "tutu", "file1"),
                                                       "This content no replacement"))

        self._test_files.append(AuxiliaryTest.TestFile(os.path.join(self._tmpdir.path(),
                                                                    "file2"),
                                                       "That content %(bla)s %(blu)s"))
        self._test_files.append(AuxiliaryTest.TestFile(os.path.join(self._tmpdir.path(),
                                                                    "tata", "file2"),
                                                       "This content %(bla)s %(blu)s"))

        self._test_files.append(AuxiliaryTest.TestFile(os.path.join(self._tmpdir.path(),
                                                                    "tata", "tutu", "file2"),
                                                       "This content %(bla)s %(blu)s"))

    def tearDown(self):
        unittest.TestCase.tearDown(self)
        del self._tmpenv
        del self._tmpdir

    def testAuxPathEnv(self):
        self.assert_(self._tmpenv["ELEMENTS_AUX_PATH"] == self._tmpdir.path())
        self.assert_(getAuxiliaryPath("file1") == os.path.join(self._tmpdir.path(), "file1"))
        self.assert_(getAuxiliaryPath("tata/file1") == os.path.join(self._tmpdir.path(), "tata", "file1"))
        self.assert_(getAuxiliaryPath("tata/tutu/file1") == os.path.join(self._tmpdir.path(), "tata", "tutu", "file1"))
        self.assert_(getAuxiliaryPath("file2") == os.path.join(self._tmpdir.path(), "file2"))
        self.assert_(getAuxiliaryPath("tata/file2") == os.path.join(self._tmpdir.path(), "tata", "file2"))
        self.assert_(getAuxiliaryPath("tata/tutu/file2") == os.path.join(self._tmpdir.path(), "tata", "tutu", "file2"))


    def testConfigure(self):
        configure("file1", self._exiting_dir)
        self.assert_(os.path.exists(os.path.join(self._exiting_dir, "file1")))
        configure("file2", self._exiting_dir, configuration={"bla":"foo", "blu":"bar"})
        target_file = os.path.join(self._exiting_dir, "file2")
        self.assert_(os.path.exists(target_file))
        self.assert_(open(target_file).read() == "That content foo bar")
        


if __name__ == "__main__":
    # import sys;sys.argv = ['', 'Test.testName']
    unittest.main()
