from pkgutil import extend_path
__path__ = extend_path(__path__, __name__)

import os.path

from .ConnectionConfiguration import *
from .DependencyConfiguration import *
from .DataSynchronizerMaker import createSynchronizer
from .DataSyncUtils import concatenatePaths, localWorkspacePrefix


class DataSync(object):
    """ A class to download test data from a data repository \
    prior to the unit test run.

    Refer to the corresponding Redmine project for more details.

    :param connectionFile: Path to the connection configuration file \
    relative to the configuration directory.

    :param dependencyFile: Path to the dependency configuration file \
    relative to the configuration directory.

    .. seealso:: https://euclid.roe.ac.uk/projects/testdata/wiki
    """

    def __init__ (self, connectionFile, dependencyFile):
        self._connectionConfig = ConnectionConfiguration(connectionFile)
        self._distantRoot = self._connectionConfig.distantRoot
        self._localRoot = self._connectionConfig.localRoot
        self._dependencyConfig = DependencyConfiguration(
                self._distantRoot, self._localRoot,
                dependencyFile)

    def download (self):
        """ Download the test data.
        """
        synchronizer = createSynchronizer(
                self._connectionConfig,
                self._dependencyConfig)
        synchronizer.downloadAllFiles()

    def downloadWithFallback (self, connectionFile):
        """ Download the test data and provide a fallback host
        in case the primary host fails.

        :param connectionFile: Path to the connection configuration file \
        of the fallback host relative to the configuration directory.
        """
        try:
            self.download()
        except Exception:
            self._connectionConfig = ConnectionConfiguration(connectionFile)
            self.download()

    def absolutePath (self, relativePath):
        """ Get the absolute path to a local test file \
        which has been downloaded.

        The absolute path is the concatenation of:

        * a system-defined prefix,
        * the local workspace,
        * the path of the file relative to the local workspace.

        On LODEEN, the prefix is empty.
        On CODEEN, it is the job workspace which the user do not know.
        It can be set by the user through the $WORKSPACE environment variable.

        .. warning:: This function must be used to access any data \
        downloaded by the DataSync tool.
        """
        return concatenatePaths([self._localRoot, relativePath])
