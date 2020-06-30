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

@author: Hubert Degaudenzi

'''

from ElementsKernel.Temporary import TempDir, TempFile
from ElementsKernel.Temporary import TempEnv
import os
from shutil import rmtree

import unittest


class TestCase(unittest.TestCase):

    """ test case for the temporary directory class """

    def setUp(self):
        unittest.TestCase.setUp(self)
        self.samplebasename = "toto"
        self.tmpdir = TempDir(suffix="tempdir", prefix=self.samplebasename)
        self.m_top_dir = TempDir()
        self.m_env = TempEnv()
        self.m_env["WORKSPACE"] = os.path.join(self.m_top_dir.path(), "work")
        
    def tearDown(self):
        unittest.TestCase.tearDown(self)
        del self.tmpdir
        del self.m_top_dir

    def testName(self):
        name1 = str(self.tmpdir)
        name2 = self.tmpdir.getName()
        # the next line should print twice the same thing
        self.assertEqual(name1, name2)

    def testDestruction(self):
        mydir = TempDir(suffix="tempdir", prefix="mydir")
        mydirname = mydir.getName()
        self.assertTrue(os.path.exists(mydirname))
        self.assertTrue(os.path.isdir(mydirname))
        del mydir
        # the temporary directory should have been removed
        self.assertFalse(os.path.exists(mydirname))

    def testContext(self):
        with TempDir() as td:
            td_path = td.getName()
            self.assertTrue(os.path.exists(td_path))
        self.assertFalse(os.path.exists(td_path))

    def testFileDestruction(self):
        myfile = TempFile()
        myfilename = myfile.getName()
        self.assertTrue(os.path.exists(myfilename))
        self.assertFalse(os.path.isdir(myfilename))
        self.assertTrue(os.path.isfile(myfilename))
        del myfile
        # the temporary directory should have been removed
        self.assertFalse(os.path.exists(myfilename))

    def testFileContext(self):
        with TempFile() as tf:
            tf_path = tf.getName()
            self.assertTrue(os.path.exists(tf_path))
        self.assertFalse(os.path.exists(tf_path))

    def testPath(self):

        with TempDir() as td:
            td_path = td.path()
            self.assertTrue(os.path.exists(td_path))

    def testEnvWith(self):

        os.environ["TESTENVEK"] = "wuk"

        with TempEnv() as e:
            e["TESTENVEK"] = "bla"
            e["TESTENVEK2"] = "bla"
            self.assertEqual(os.environ.get("TESTENVEK"), "bla")

        # The environment is restored
        self.assertEqual(os.environ.get("TESTENVEK"), "wuk")

        with TempEnv() as e:
            e["TESTENVEK2"] = "bla"
            self.assertEqual(os.environ.get("TESTENVEK2"), "bla")

        self.assertEqual(os.environ.get("TESTENVEK2", None), None)
        
    def testKeepTmpDir(self):
        
        os.environ["KEEPTEMPDIR"] = "1"
        
        with TempDir() as td:
            td_path = td.path()
            self.assertTrue(os.path.exists(td_path))
            
        self.assertTrue(os.path.exists(td_path))
        rmtree(td_path)
        self.assertFalse(os.path.exists(td_path))
        del os.environ["KEEPTEMPDIR"]

    def testTempEnv2(self):
        
        self.assertEqual(self.m_env["WORKSPACE"], os.path.join(self.m_top_dir.path(), "work"))
        
        with TempEnv() as local:
            self.assertEqual(self.m_env["WORKSPACE"], os.path.join(self.m_top_dir.path(), "work"))
            self.assertEqual(local["WORKSPACE"], os.path.join(self.m_top_dir.path(), "work"))
            local["WORKSPACE"] = "that_work"
            self.assertEqual(self.m_env["WORKSPACE"], "that_work")
            self.assertEqual(local["WORKSPACE"], "that_work")
        
        self.assertEqual(self.m_env["WORKSPACE"], os.path.join(self.m_top_dir.path(), "work"))
            
        
if __name__ == '__main__':
    unittest.main()
