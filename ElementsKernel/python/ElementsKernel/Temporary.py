""" create temporary structure for remote buids """

from shutil import rmtree

import os
import tempfile
if not 'mkdtemp' in dir(tempfile):
    # mkdtemp has been introduced in python 2.3, I simulate it
    import warnings
    warnings.filterwarnings(
        action='ignore', message='.*tmpnam.*', category=RuntimeWarning)

    def mkdtemp():
        name = os.tmpnam()  # @UndefinedVariable
        os.mkdir(name, 0700)
        return name

    def mkstemp():
        name = os.tmpnam()  # @UndefinedVariable
        return (os.open(name, os.O_CREAT | os.O_RDWR | os.O_EXCL, 0600),
                name)
else:
    # use the real mkdtemp
    from tempfile import mkdtemp, mkstemp


class TempResource(object):
    def __init__(self):
        """ Constructor """
        self._name = None
        self._get_resource()

    def getName(self):
        """Returns the name of the temporary directory"""
        return self._name

    def path(self):
        return self._name

    def __str__(self):
        """Convert to string."""
        return self.getName()

    def __del__(self):
        """Destructor.

        Remove the temporary directory.
        """
        self._del_resource()

    def __enter__(self):
        """ To work with the context"""
        self._get_resource()
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        """ function called at the end of a context """
        self._del_resource()

    def __getattr__(self, attr):
        return getattr(self._file, attr)


class TempDir(TempResource):

    """Class to create a temporary directory."""

    def __init__(self, suffix="", prefix="tmp", base_dir=None, keep_var="KEEPTEMPDIR"):
        """Constructor.

        'keep_var' is used to define which environment variable will prevent the
        deletion of the directory.

        The other arguments are the same as tempfile.mkdtemp.
        """
        self._keep_var = keep_var
        self._suffix = suffix
        self._prefix = prefix
        self._base_dir = base_dir
        super(TempDir, self).__init__()

    def _get_resource(self):
        """ Internal function to get the resource"""
        if not self._name:
            self._name = mkdtemp(self._suffix, self._prefix, self._base_dir)

    def _del_resource(self):
        """Internal function to remove the resource"""
        if self._name:
            if self._keep_var in os.environ:
                import logging
                logging.info("%s set: I do not remove the temporary directory '%s'",
                             self._keep_var, self._name)
            else:
                rmtree(self._name)
                self._name = None



class TempFile(TempResource):

    """ class to create a temporary file """

    def __init__(self):
        """ Constructor """
        self._file = None
        super(TempFile, self).__init__()


    def _get_resource(self):
        """ Internal function to get the resource"""
        if not self._name:
            fd, self._name = mkstemp()
            self._file = os.fdopen(fd, "w+")

    def _del_resource(self):
        """ Internal function to remove the resource """
        if self._name:
            self._file.close()
            self._file = None
            os.remove(self._name)
            self._name = ''

