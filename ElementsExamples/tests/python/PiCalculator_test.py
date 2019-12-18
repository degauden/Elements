import unittest

class PiCalculatorTestCase(unittest.TestCase):

    def setUp(self):
        unittest.TestCase.setUp(self)
        try:
            import PiCalculator
            self.has_cython = True
        except:
            self.has_cython = False

    def tearDown(self):
        unittest.TestCase.tearDown(self)

    def testPrintPi(self):
        if self.has_cython:
            from PiCalculator import print_pi


if __name__ == '__main__':
    unittest.main()
