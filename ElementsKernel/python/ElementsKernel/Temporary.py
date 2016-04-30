""" create temporary structure for remote buids """

from shutil import rmtree

import os
import tempfile
import sys
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

from ElementsKernel import Logging


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
                log = Logging.getLogger(None)
                log.info("%s set: I do not remove the temporary directory '%s'",
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



class Environment(object):

    """
    Class to changes the environment temporarily.
    """

    def __init__(self, orig=os.environ, keep_same=False):
        """
        Create a temporary environment on top of the one specified
        (it can be another TemporaryEnvironment instance).
        """
        # print "New environment"
        self.old_values = {}
        self._orig = orig
        self.env = None
        self._keep_same = keep_same
        self._aq_env()

        # the keys of the environment dictionary are case insensitive on
        # windows
        if sys.platform.startswith("win"):
            self._fixKey = lambda key: key.upper()
        else:
            self._fixKey = lambda key: key

    def _aq_env(self):
        if not self.env:
            self.env = self._orig

    def __setitem__(self, key, value):
        """
        Set an environment variable recording the previous value.
        """
        key = self._fixKey(key)
        if key not in self.old_values:
            if key in self.env:
                if not self._keep_same or self.env[key] != value:
                    self.old_values[key] = self.env[key]
            else:
                self.old_values[key] = None
        self.env[key] = value

    def __getitem__(self, key):
        """
        Get an environment variable.
        Needed to provide the same interface as os.environ.
        """
        key = self._fixKey(key)
        return self.env[key]

    def __delitem__(self, key):
        """
        Unset an environment variable.
        Needed to provide the same interface as os.environ.
        """
        key = self._fixKey(key)
        if key not in self.env:
            raise KeyError(key)
        self.old_values[key] = self.env[key]
        del self.env[key]
        log = Logging.getLogger(None)
        log.info("removed %s from environment" % key)

    def keys(self):
        """
        Return the list of defined environment variables.
        Needed to provide the same interface as os.environ.
        """
        return self.env.keys()

    def has_key(self, key):
        """
        return True if the key is present
        """
        key = self._fixKey(key)
        return key in self.env.keys()

    def items(self):
        """
        Return the list of (name,value) pairs for the defined environment variables.
        Needed to provide the same interface as os.environ.
        """
        return self.env.items()

    def __contains__(self, key):
        """
        Operator 'in'.
        Needed to provide the same interface as os.environ.
        """
        key = self._fixKey(key)
        return key in self.env

    def restore(self):
        """
        Revert all the changes done to the original environment.
        """
        for key, value in self.old_values.items():
            if value is None:
                del self.env[key]
            else:
                self.env[key] = value
        self.old_values = {}

    def __del__(self):
        """
        Revert the changes on destruction.
        """
        self.restore()

    def __enter__(self):
        """ To work with the context"""
        self._aq_env()
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        """
        Revert the changes on the end of the context.
        """
        self.restore()

    def get(self, key, default=None):
        """
        Implementation of the standard get method of a dictionary: return the
        value associated to "key" if present, otherwise return the default.
        """
        key = self._fixKey(key)
        return self.env.get(key, default)

    def commit(self):
        """
        Forget the old values for the changes done so far (avoids that the
        changes are rolled-back when the instance goes out of scope).
        """
        self.old_values = {}

    def gen_script(self, shell_type):
        """
        Generate a shell script to reproduce the changes in the environment.
        """
        shells = ['csh', 'sh', 'bat']
        if shell_type not in shells:
            raise RuntimeError(
                "Shell type '%s' unknown. Available: %s" % (shell_type, shells))
        out = ""
        for key in self.old_values:
            if key not in self.env:
                # unset variable
                if shell_type == 'csh':
                    out += 'unsetenv %s\n' % key
                elif shell_type == 'sh':
                    out += 'unset %s\n' % key
                elif shell_type == 'bat':
                    out += 'set %s=\n' % key
            else:
                # set variable
                if shell_type == 'csh':
                    out += 'setenv %s "%s"\n' % (key, self.env[key])
                elif shell_type == 'sh':
                    out += 'export %s="%s"\n' % (key, self.env[key])
                elif shell_type == 'bat':
                    out += 'set %s=%s\n' % (key, self.env[key])
        return out

