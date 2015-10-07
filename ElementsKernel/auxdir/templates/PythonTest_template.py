"""
@file %(FILE)s
@date %(DATE)s
@author %(AUTHOR)s
"""
import py.test
import %(MODULENAME)s.%(PYTHONMODULE)s

class Test%(PYTHONMODULE)s(object):
    """
    !!! Test class example for python             !!!
    !!! Please remove it and add your tests there !!! 
    """
    def testFailure(self):
        assert False, "!!!! Please implement your tests !!!!"
