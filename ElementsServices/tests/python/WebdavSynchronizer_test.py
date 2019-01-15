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


import py.test

from ElementsServices.DataSync import ConnectionConfiguration
from ElementsServices.DataSync import DependencyConfiguration
from ElementsServices.DataSync.WebdavSynchronizer import WebdavSynchronizer

from fixtures.ConfigFilesFixture import *
from fixtures.TestDataSynchronizer import TestDataSynchronizer


class TestWebdavSynchronizer(TestDataSynchronizer):

    def createTestSynchronizer(self):
        connection = ConnectionConfiguration(theWebdavFrConfig())
        distantRoot = connection.distantRoot
        localRoot = connection.localRoot
        dependencies = DependencyConfiguration(
            distantRoot, localRoot, theDependencyConfig())
        return WebdavSynchronizer(connection, dependencies)

    def test_webdavGetCmd(self):
        distantFile = "src/distant_file.fits"
        localFile = "dst/local_file.fits"
        synchronizer = self.createTestSynchronizer()
        cmd = synchronizer.createDownloadCommand(distantFile, localFile)
        assert "wget " in cmd, "wget not found in WebDAV command: " + cmd
        assert "-O" in cmd, "-O option not specified in WebDAV command: " + cmd
        assert localFile in cmd, "Local path not found in WebDAV command: " + cmd
        assert distantFile in cmd, "Distant path not fount in WebDAV command: " + cmd
        assert "8" in cmd, "Retries not found in WebDAV command: " + cmd

    def test_webdavFixture(self):
        webdavFR = theWebdavFrConfig()
        self.checkSynchronization(webdavFR)
        self.checkDownloadTestData(webdavFR)

          #TODO should we validate in all SDCs?
#         webdavES = theWebdavEsConfig()
#         self.checkSynchronization(webdavES)
#         self.checkDownloadTestData(webdavES)

    def test_downloadErrorReport(self):
        webdavFR = theWebdavFrConfig()
        self.checkDownloadErrorReport(webdavFR)
