import unittest

class SwigExampleTestCase(unittest.TestCase):

    def setUp(self):
        unittest.TestCase.setUp(self)
        try:
            import SwigExample
            self.has_swig = True
        except:
            self.has_swig = False

    def tearDown(self):
        unittest.TestCase.tearDown(self)

    def testFuncSwig(self):
        if self.has_swig:
            from SwigExample import functionExample
            self.assertEqual(functionExample(3), 9)

    def testClassSwig(self):
        if self.has_swig:
            from SwigExample import SwigClassExample
            a = SwigClassExample(2.0, 3.0)
            self.assertEqual(a.getRa(), 2.0)
            self.assertEqual(a.getDec(), 3.0)

if __name__ == '__main__':
    unittest.main()
