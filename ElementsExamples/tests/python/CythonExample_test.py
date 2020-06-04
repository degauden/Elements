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
:date: Jun 14, 2017

:author: hubert
'''
import unittest

from ElementsKernel.Path import pyVersionWhich

cython_exe = pyVersionWhich("cython")

if cython_exe:
    from PiCalculator import print_pi
    from Rect import Rectangle

    class RectangleTest(unittest.TestCase):
        """ Rectangle class to test Cython """

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
    unittest.main()
