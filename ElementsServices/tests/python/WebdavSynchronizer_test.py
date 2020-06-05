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

from ElementsServices.DataSync import ConnectionConfiguration
from ElementsServices.DataSync import DependencyConfiguration
from ElementsServices.DataSync import WebdavSynchronizer

from fixtures.ConfigFilesFixture import *
from fixtures.TestDataSynchronizer import TestDataSynchronizer


class TestWebdavSynchronizer(TestDataSynchronizer):

    def createTestSynchronizer(self):
        connection = ConnectionConfiguration(theWebdavFrConfig())
        distant_root = connection.distant_root
        local_root = connection.local_root
        dependencies = DependencyConfiguration(
            distant_root, local_root, theDependencyConfig())
        return WebdavSynchronizer.WebdavSynchronizer(connection, dependencies)

    def test_webdavGetCmd(self):
        distant_file = "src/distant_file.fits"
        local_file = "dst/local_file.fits"
        synchronizer = self.createTestSynchronizer()
        cmd = synchronizer.createDownloadCommand(distant_file, local_file)
        assert "wget " in cmd, "wget not found in WebDAV command: " + cmd
        assert "-O" in cmd, "-O option not specified in WebDAV command: " + cmd
        assert local_file in cmd, "Local path not found in WebDAV command: " + cmd
        assert distant_file in cmd, "Distant path not fount in WebDAV command: " + cmd
        assert "8" in cmd, "Retries not found in WebDAV command: " + cmd

    def test_webdavFixture(self):
        if not WebdavSynchronizer.webdavIsInstalled():
            return
        webdav_fr = theWebdavFrConfig()
        self.checkSynchronization(webdav_fr)
        self.checkDownloadTestData(webdav_fr)

          #TODO should we validate in all SDCs?
#         webdavES = theWebdavEsConfig()
#         self.checkSynchronization(webdavES)
#         self.checkDownloadTestData(webdavES)

    def test_downloadErrorReport(self):
        if not WebdavSynchronizer.webdavIsInstalled():
            return
        webdav_fr = theWebdavFrConfig()
        self.checkDownloadErrorReport(webdav_fr)
