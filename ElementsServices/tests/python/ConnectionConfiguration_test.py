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

import os
import unittest

from ElementsKernel.Temporary import TempDir, TempEnv
from ElementsServices.DataSync import DataHost, ConnectionConfiguration

from fixtures.ConfigFilesFixture import theWebdavFrConfig, theNoOverwriteConfig, thePrefixedLocalWorkspace


class TestConnectionConfiguration(unittest.TestCase):

    def setUp(self):
        unittest.TestCase.setUp(self)
        self.m_top_dir = TempDir(prefix="DataSync_test")
        self.m_env = TempEnv()
        self.m_env["WORKSPACE"] = os.path.join(self.m_top_dir.path(), "workspace")

    def tearDown(self):
        unittest.TestCase.tearDown(self)
        del self.m_top_dir


    def checkHostParsing(self, host, names):
        config = ConnectionConfiguration()
        for name in names:
            config.parseHost(name)
            assert config.host == host, "'" + name + "' not parsed as " + host.name

    def test_hostParsing(self):
        self.checkHostParsing(DataHost.IRODS, ["irods", "iRODS", "IRODS"])
        self.checkHostParsing(DataHost.WEBDAV, ["webdav", "WebDAV", "WEBDAV"])
        self.checkHostParsing(None, ["blob", "Blob", "BLOB"])

    def test_overwritingPolicy(self):
        config = ConnectionConfiguration(theWebdavFrConfig())
        config_no_overwrite = ConnectionConfiguration(theNoOverwriteConfig())
        assert config.overwritingAllowed(), \
            "Overwrite should be allowed, but it is not"
        assert not config_no_overwrite.overwritingAllowed(), \
            "Overwrite should not be allowed, but it is"

    def test_webdavFrConfig(self):
        config = ConnectionConfiguration(theWebdavFrConfig())
        assert config.host == DataHost.WEBDAV, \
            "Host shoud be WebDAV, but it is not"
        assert config.overwriting_policy, \
            "Overwrite should be allowed, but it is not"
        assert config.distant_root == "/euclid-fr/ct/mock_test_data", \
            "Distant workspace path misread: " + config.distant_root
        assert config.local_root == thePrefixedLocalWorkspace(), \
            "Local workspace path misread: " + config.local_root
        assert config.tries == 8, \
            "Wrong number of tries: " + str(config.tries)
