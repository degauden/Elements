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

from ElementsKernel.Temporary import TempDir, TempFile
from ElementsKernel.Temporary import TempEnv
import os

import unittest


class TestCase(unittest.TestCase):

    """ test case for the temporary directory class """

    def setUp(self):
        unittest.TestCase.setUp(self)
        self.samplebasename = "toto"
        self.tmpdir = TempDir(suffix="tempdir", prefix=self.samplebasename)

    def tearDown(self):
        unittest.TestCase.tearDown(self)
        del self.tmpdir

    def testName(self):
        name1 = str(self.tmpdir)
        name2 = self.tmpdir.getName()
        # the next line should print twice the same thing
        self.assert_(name1 == name2)

    def testDestruction(self):
        mydir = TempDir(suffix="tempdir", prefix="mydir")
        mydirname = mydir.getName()
        self.assert_(os.path.exists(mydirname))
        self.assert_(os.path.isdir(mydirname))
        del mydir
        # the temporary directory should have been removed
        self.assert_(not os.path.exists(mydirname))

    def testContext(self):
        with TempDir() as td:
            td_path = td.getName()
            self.assert_(os.path.exists(td_path))
        self.assert_(not os.path.exists(td_path))

    def testFileDestruction(self):
        myfile = TempFile()
        myfilename = myfile.getName()
        self.assert_(os.path.exists(myfilename))
        self.assert_(not os.path.isdir(myfilename))
        self.assert_(os.path.isfile(myfilename))
        del myfile
        # the temporary directory should have been removed
        self.assert_(not os.path.exists(myfilename))

    def testFileContext(self):
        with TempFile() as tf:
            tf_path = tf.getName()
            self.assert_(os.path.exists(tf_path))
        self.assert_(not os.path.exists(tf_path))

    def testPath(self):

        with TempDir() as td:
            td_path = td.path()
            self.assert_(os.path.exists(td_path))

    def testEnvWith(self):

        os.environ["TESTENVEK"] = "wuk"

        with TempEnv() as e:
            e["TESTENVEK"] = "bla"
            e["TESTENVEK2"] = "bla"
            self.assert_(os.environ.get("TESTENVEK") == "bla")

        # The environment is restored
        self.assert_(os.environ.get("TESTENVEK") == "wuk")

        with TempEnv() as e:
            e["TESTENVEK2"] = "bla"
            self.assert_(os.environ.get("TESTENVEK2") == "bla")

        self.assert_(os.environ.get("TESTENVEK2", None) == None)

if __name__ == '__main__':
    unittest.main()
