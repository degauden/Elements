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

from abc import abstractmethod, ABCMeta

import os.path

from .DataSyncUtils import createLocalDirOf, runCommandAndCaptureOutErr


class DownloadFailed (Exception):
    """An exception raised when downloading fails.
    """

    def __init__ (self, distant_file, local_file):
        super(DownloadFailed, self).__init__()
        self.message = "Unable to download file: '" + distant_file + "' as: '" + local_file + "'."


class DataSynchronizer(object):
    """A data synchronizer class is able to download
    test data from a host.

    This is the abstract class to be extended for each data host.
    """
    __metaclass__ = ABCMeta

    def __init__ (self,
            connection,
            dependencies):
        self._connection = connection
        self._file_map = dependencies.getFileMap()

    def downloadAllFiles (self):
        """Download all the test files.
        """
        for local_file, distant_file in self._file_map.items():
            if self.fileShouldBeWritten(local_file):
                self.downloadOneFile(distant_file, local_file)

    def fileShouldBeWritten (self, local_file):
        """Check whether a file should be locally written.
        """
        if not self.fileAlreadyExists(local_file):
            return True
        return self._connection.overwritingAllowed()

    def fileAlreadyExists (self, local_file):
        """Check whether a file to be downloaded already exists locally.
        """
        return os.path.isfile(local_file)

    def downloadOneFile (self, distant_file, local_file):
        """Download a given test file.
        """
        command = self.createDownloadCommand(distant_file, local_file)
        createLocalDirOf(local_file)
        _out, _err = runCommandAndCaptureOutErr(command)
        if not self.hasBeenDownloaded(distant_file, local_file):
            raise DownloadFailed(distant_file, local_file)
            # TODO output _out, _err

    def hasBeenDownloaded (self, distant_file, local_file):
        """Check whether a given test file has been downloaded,
        i.e., exists and is not empty.
        """
        if not os.path.isfile(local_file):
            return False
        return os.path.getsize(local_file) > 0

    @abstractmethod
    def createDownloadCommand (self, distant_file, local_file):
        """Create the command to download a file.
        """

