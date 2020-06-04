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

""" test module for the class example"""

import unittest
from ElementsExamples.PythonModuleExample import ClassExample


class ClassExampleTestCase(unittest.TestCase):
    """
    Simple test case for the ClassExample class
    """

    def setUp(self):
        unittest.TestCase.setUp(self)
        self.tol = 1e-6
        self.first = 23.4857
        self.second = 3.4756
        self.my_list = [6, 7, 8, 9]
        self.expected_result = 6 + 7 + 8 + 9
        self.example_object = ClassExample(self.my_list)

    def tearDown(self):
        unittest.TestCase.tearDown(self)

    def testProduct(self):
        result = ClassExample.product(self.first, self.second)
        assert abs(result - (self.first * self.second)) < self.tol

    def testDestruction(self):
        assert abs(self.expected_result - self.example_object.sumListValues()) < self.tol


if __name__ == '__main__':
    unittest.main()

