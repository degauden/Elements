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

    def testClassPositionalArgsSwig(self):
        if self.has_swig:
            from SwigExample import SwigClassExample
            a = SwigClassExample(dec = 3.0, ra = 2.0)
            self.assertEqual(a.getRa(), 2.0)
            self.assertEqual(a.getDec(), 3.0)

if __name__ == '__main__':
    unittest.main()
