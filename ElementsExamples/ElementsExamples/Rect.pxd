cimport ElementsExamples.CRectangle
from ElementsExamples.CRectangle cimport Rectangle as CRectangle

cdef class Rectangle:
    cdef ElementsExamples.CRectangle.Rectangle *thisptr      # hold a C++ instance which we're wrapping
    cpdef getLength(self)
    cpdef getHeight(self)
    cpdef getArea(self)
    cpdef move(self, dx, dy)
