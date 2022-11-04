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
import pytest
import unittest

from ElementsKernel.Temporary import TempDir, TempEnv

from ElementsServices.DataSync import DataSync
from ElementsServices.DataSync.IrodsSynchronizer import irodsIsInstalled
from ElementsServices.DataSync.WebdavSynchronizer import webdavIsInstalled

from fixtures.ConfigFilesFixture import theDependencyConfig, theLocalFiles
from fixtures.ConfigFilesFixture import aBadConnectionConfig, theWebdavFrConfig
from fixtures.ConfigFilesFixture import theIrodsFrConfig


class TestDataSync(unittest.TestCase):

    def setUp(self):
        unittest.TestCase.setUp(self)
        self.m_top_dir = TempDir(prefix="DataSync_test")
        self.m_env = TempEnv()
        self.m_env["WORKSPACE"] = os.path.join(self.m_top_dir.path(), "workspace")

    def tearDown(self):
        unittest.TestCase.tearDown(self)
        del self.m_top_dir

    def checkDownload(self, connection_config):
        sync = DataSync(connection_config, theDependencyConfig())
        sync.download()
        for file in theLocalFiles():
            abs_path = sync.absolutePath(file)
            assert os.path.isfile(abs_path)
            os.remove(abs_path)

    def checkFallback(self, fallback_config):
        sync = DataSync(aBadConnectionConfig(), theDependencyConfig())
        with pytest.raises(Exception):
            sync.download()
        sync.downloadWithFallback(fallback_config)
        for file in theLocalFiles():
            abs_path = sync.absolutePath(file)
            assert os.path.isfile(abs_path)
            os.remove(abs_path)

    def test_DataSyncWebdavFr(self):
        if webdavIsInstalled():
            self.checkDownload(theWebdavFrConfig())
            self.checkFallback(theWebdavFrConfig())

    def test_DataSyncIrodsFr(self):
        if irodsIsInstalled():
            self.checkDownload(theIrodsFrConfig())
            self.checkFallback(theIrodsFrConfig())
