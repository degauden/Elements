""" Definition of a python package using extend_path
    to create a namespace
"""


from pkgutil import extend_path
__path__ = extend_path(__path__, __name__)  # @ReservedAssignment
