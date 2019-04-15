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


import os.path
import py.test

from ElementsServices.DataSync import DataSync
from ElementsServices.DataSync.IrodsSynchronizer import irodsIsInstalled
from ElementsServices.DataSync.WebdavSynchronizer import webdavIsInstalled

from fixtures.ConfigFilesFixture import *


class TestDataSync(object):

    def checkDownload(self, connectionConfig):
        sync = DataSync(connectionConfig, theDependencyConfig())
        sync.download()
        for file in theLocalFiles():
            absPath = sync.absolutePath(file)
            assert os.path.isfile(absPath)
            os.remove(absPath)

    def checkFallback(self, fallbackConfig):
        sync = DataSync(aBadConnectionConfig(), theDependencyConfig())
        with py.test.raises(Exception):
            sync.download()
        sync.downloadWithFallback(fallbackConfig)
        for file in theLocalFiles():
            absPath = sync.absolutePath(file)
            assert os.path.isfile(absPath)
            os.remove(absPath)

    def test_DataSyncWebdavFr(self):
        if webdavIsInstalled():
            self.checkDownload(theWebdavFrConfig())
            self.checkFallback(theWebdavFrConfig())

    def test_DataSyncIrodsFr(self):
        if irodsIsInstalled():
            self.checkDownload(theIrodsFrConfig())
            self.checkFallback(theIrodsFrConfig())
