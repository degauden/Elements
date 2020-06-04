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

cimport ElementsExamples.CRectangle

from ElementsExamples cimport CRectangle

cdef class Rectangle:

    def __cinit__(self, int x0, int y0, int x1, int y1):
#         self.thisptr = new ElementsExamples.c_rectangle.Rectangle(x0, y0, x1, y1)
        self.thisptr = new CRectangle.Rectangle(x0, y0, x1, y1)

    def __dealloc__(self):
        del self.thisptr

    cpdef getLength(self):
        return self.thisptr.getLength()

    cpdef getHeight(self):
        return self.thisptr.getHeight()

    cpdef getArea(self):
        return self.thisptr.getArea()

    cpdef move(self, dx, dy):
        self.thisptr.move(dx, dy)
