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


from abc import ABC, abstractmethod
import os.path

from .ConnectionConfiguration import *
from .DependencyConfiguration import *
from .DataSyncUtils import *


class DownloadFailed (Exception):
    """An exception raised when downloading fails.
    """
    def __init__ (self, distantFile:str, localFile:str):
        self.message = "Unable to download file: '" + distantFile + "' as: '" + localFile + "'."


class DataSynchronizer(ABC):
    """A data synchronizer class is able to download
    test data from a host.

    This is the abstract class to be extended for each data host.
    """

    def __init__ (self,
            connection:ConnectionConfiguration,
            dependencies:DependencyConfiguration):
        self._connection = connection
        self._fileMap = dependencies.getFileMap()

    def downloadAllFiles (self):
        """Download all the test files.
        """
        for localFile, distantFile in self._fileMap.items():
            if self.fileShouldBeWritten(localFile):
                self.downloadOneFile(distantFile, localFile)

    def fileShouldBeWritten (self, localFile:str):
        """Check whether a file should be locally written.
        """
        if not self.fileAlreadyExists(localFile):
            return True
        return self._connection.overwritingAllowed()

    def fileAlreadyExists (self, localFile:str):
        """Check whether a file to be downloaded already exists locally.
        """
        return os.path.isfile(localFile)

    def downloadOneFile (self, distantFile:str, localFile:str):
        """Download a given test file.
        """
        command = self.createDownloadCommand(distantFile, localFile)
        createLocalDirOf(localFile)
        out, err = runCommandAndCaptureOutErr(command)
        if not self.hasBeenDownloaded(distantFile, localFile):
            raise DownloadFailed(distantFile, localFile)
            #TODO output out, err

    def hasBeenDownloaded (self, distantFile:str, localFile:str):
        """Check whether a given test file has been downloaded,
        i.e., exists and is not empty.
        """
        if not os.path.isfile(localFile):
            return False
        return os.path.getsize(localFile) > 0

    @abstractmethod
    def createDownloadCommand (self, distantFile:str, localFile:str):
        """Create the command to download a file.
        """
        pass
