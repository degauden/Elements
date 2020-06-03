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

cdef extern from "ElementsExamples/PiCalculator.h" namespace "Elements::Examples":
    cdef cppclass PiCalculator:
        void calculate(unsigned int terms) nogil
        void setShowResultCallback(void * callback)

cdef void show_result_callback(double pi) with gil:
    print(float(pi))

def print_pi(terms):
    cdef PiCalculator pi_calc
    pi_calc.setShowResultCallback(&show_result_callback)
    cdef unsigned int terms_int = terms
    with nogil:
        pi_calc.calculate(terms_int)
