from ElementsKernel.Temporary import TempDir, TempFile
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


if __name__ == '__main__':
    unittest.main()
