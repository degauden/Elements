'''
Created on Jun 14, 2017

@author: hubert
'''
import unittest

from PiCalculator import print_pi
from Rect import Rectangle

class RectangleTest(unittest.TestCase):

    def setUp(self):
        self.rectangle = Rectangle(1, 2, 3, 4)

    def test_getLength(self):
        self.assertEqual(self.rectangle.getLength(), 2)

    def test_getHeight(self):
        self.assertEqual(self.rectangle.getHeight(), 2)

    def test_getArea(self):
        self.assertEqual(self.rectangle.getArea(), 4)

    def test_move(self):
        self.rectangle.move(4, 8)
        self.test_getLength()
        self.test_getHeight()
        self.test_getArea()

if __name__ == "__main__":
    # import sys;sys.argv = ['', 'Test.testName']
    unittest.main()
