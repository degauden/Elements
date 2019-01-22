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


import py.test
import os.path

from ElementsServices.DataSync.DataSyncUtils import dataSyncConfFilePath
from ElementsServices.DataSync import ConnectionConfiguration, DependencyConfiguration
from ElementsServices.DataSync import DataSync
from ElementsServices.DataSync import DataSynchronizer
from ElementsServices.DataSync.DataSynchronizer import DownloadFailed
from ElementsServices.DataSync.DataSynchronizerMaker import createSynchronizer

from fixtures.ConfigFilesFixture import *


class TestDataSynchronizer (object):

    def checkSynchronization(self, connection):
        capture_manager = py.test.config.pluginmanager.getplugin(
            'capturemanager')
        try:
            capture_manager.suspendcapture(in_=True)
            # TODO why is this ugly thing needed with EDEN 2.0?
        except:
            pass

        dependency = theDependencyConfig()
        connectionConfig = ConnectionConfiguration(connection)
        distantRoot = connectionConfig.distantRoot
        localRoot = connectionConfig.localRoot
        dependencyConfig = DependencyConfiguration(
            distantRoot, localRoot, dependency)
        synchronizer = createSynchronizer(connectionConfig, dependencyConfig)
        synchronizer.downloadAllFiles()
        fileList = dependencyConfig.getLocalPaths()
        for f in fileList:
            assert os.path.isfile(f), "File not found: " + f + " using command: " \
                + synchronizer.createDownloadCommand(dependencyConfig.getFileMap()[f], f)
            os.remove(f)

    def checkDownloadTestData(self, connection):
        dependency = theDependencyConfig()
        sync = DataSync(connection, dependency)
        sync.download()
        connectionConfig = ConnectionConfiguration(connection)
        distantRoot = connectionConfig.distantRoot
        localRoot = connectionConfig.localRoot
        dependencyConfig = DependencyConfiguration(
            distantRoot, localRoot, dependency)
        fileList = dependencyConfig.getLocalPaths()
        for f in fileList:
            assert os.path.isfile(f), "File not found: " + f
            os.remove(f)

    def checkDownloadErrorReport(self, connection):
        dependency = dataSyncConfFilePath(theInvalidDependencyConfig())
        with py.test.raises(DownloadFailed):
            sync = DataSync(connection, dependency)
            sync.download()
