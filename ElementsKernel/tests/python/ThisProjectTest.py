import unittest

from ThisProject import THIS_PROJECT_NAME

class ThisProjectCase(unittest.TestCase):

    def setUp(self):
        unittest.TestCase.setUp(self)

    def tearDown(self):
        unittest.TestCase.tearDown(self)

    def testName(self):
        self.assert_(THIS_PROJECT_NAME=="Elements")

if __name__ == '__main__':
    unittest.main()