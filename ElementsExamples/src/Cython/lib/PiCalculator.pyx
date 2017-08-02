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
