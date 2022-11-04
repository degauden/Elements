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

import pytest
import os.path

from ElementsServices.DataSync.DataSyncUtils import dataSyncConfFilePath
from ElementsServices.DataSync import ConnectionConfiguration, DependencyConfiguration
from ElementsServices.DataSync import DataSync
from ElementsServices.DataSync.DataSynchronizer import DownloadFailed
from ElementsServices.DataSync.DataSynchronizerMaker import createSynchronizer

from fixtures.ConfigFilesFixture import theDependencyConfig, theInvalidDependencyConfig


class TestDataSynchronizer (object):

    @classmethod
    def checkSynchronization(cls, connection):
        dependency = theDependencyConfig()
        connection_config = ConnectionConfiguration(connection)
        distant_root = connection_config.distant_root
        local_root = connection_config.local_root
        dependency_config = DependencyConfiguration(
            distant_root, local_root, dependency)
        synchronizer = createSynchronizer(connection_config, dependency_config)
        synchronizer.downloadAllFiles()
        file_list = dependency_config.getLocalPaths()
        for f in file_list:
            assert os.path.isfile(f), "File not found: " + f + " using command: " \
                +synchronizer.createDownloadCommand(dependency_config.getFileMap()[f], f)
            os.remove(f)

    @classmethod
    def checkDownloadTestData(cls, connection):
        dependency = theDependencyConfig()
        sync = DataSync(connection, dependency)
        sync.download()
        connection_config = ConnectionConfiguration(connection)
        distant_root = connection_config.distant_root
        local_root = connection_config.local_root
        dependency_config = DependencyConfiguration(
            distant_root, local_root, dependency)
        fileList = dependency_config.getLocalPaths()
        for f in fileList:
            assert os.path.isfile(f), "File not found: " + f
            os.remove(f)

    @classmethod
    def checkDownloadErrorReport(cls, connection):
        dependency = dataSyncConfFilePath(theInvalidDependencyConfig())
        with pytest.raises(DownloadFailed):
            sync = DataSync(connection, dependency)
            sync.download()
