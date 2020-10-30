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

""" Small example for a python module

"""


import ElementsKernel.Logging as log

class ClassExample(object):
    """
    This is an example of the most basic Python class
    """
    logger = log.getLogger('ClassExample')
    def __init__(self, my_list):
        """
        Constructor
        """
        self._m_i = 5
        self._m_list = my_list

    def sumListValues(self):
        """
        Method summing the elements of the list
        """
        my_sum = 0
        for element in self._m_list:
            my_sum += element
        return my_sum


    @staticmethod
    def product(first, second):
        """
        This is an example static method

        :param first: The first number
        :param second: The second number

        :return: The product of the two numbers
        """
        return first * second
