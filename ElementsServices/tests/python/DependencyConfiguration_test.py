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

import unittest
import os

from ElementsKernel.Temporary import TempDir, TempEnv

from ElementsServices.DataSync import DependencyConfiguration
from ElementsServices.DataSync.DataSyncUtils import getConfigurationPath, localWorkspacePrefix, concatenatePaths

from fixtures.ConfigFilesFixture import theDependencyConfig, thePrefixedLocalWorkspace, theLocalFiles


class TestDependencyConfiguration(unittest.TestCase):

    def setUp(self):
        unittest.TestCase.setUp(self)
        self.m_top_dir = TempDir(prefix="DataSync_test")
        self.m_env = TempEnv()
        self.m_env["WORKSPACE"] = os.path.join(self.m_top_dir.path(), "workspace")

    def tearDown(self):
        unittest.TestCase.tearDown(self)
        del self.m_top_dir

    def test_parseLineWithoutAlias(self):
        distant_root = "distant/"
        local_root = "local/"
        config = DependencyConfiguration(distant_root, local_root)
        file = "test/file.fits"
        assert not config.lineHasAlias(file)
        config.parseLineWithoutAlias(file)
        assert config.getDependencyCount() == 1
        assert config.getDistantPaths()[0] == distant_root + file
        abs_local_file = concatenatePaths([localWorkspacePrefix(), local_root, file])
        assert config.getLocalPaths()[0] == abs_local_file
        assert config.getDistantPathOf(abs_local_file) == distant_root + file

    def test_parseLineWithAlias(self):
        distant_root = "distant/"
        local_root = "local/"
        config = DependencyConfiguration(distant_root, local_root)
        distant = "test/file_v1.fits"
        local = "test/file.fits"
        line = distant + config.getAliasSeparator() + local
        assert config.lineHasAlias(line)
        config.parseLineWithAlias(line)
        assert config.getDependencyCount() == 1
        assert config.getDistantPaths()[0] == distant_root + distant
        abs_local_file = concatenatePaths([localWorkspacePrefix(), local_root, local])
        assert config.getLocalPaths()[0] == abs_local_file
        assert config.getDistantPathOf(abs_local_file) == distant_root + distant

    def test_confDependencies(self):
        the_config_file = getConfigurationPath(theDependencyConfig())
        dependencies = DependencyConfiguration(
            "", thePrefixedLocalWorkspace(),
            the_config_file)
        parsed_files = dependencies.getLocalPaths()
        assert len(parsed_files) == len(theLocalFiles()), "Input cardinal is different from read one"
        the_local_abs_paths = [
            concatenatePaths([thePrefixedLocalWorkspace(), f])
            for f in theLocalFiles()]
        for f in the_local_abs_paths:
            assert f in parsed_files, "Input file not read"
        for f in parsed_files:
            assert f in the_local_abs_paths, "File not in input list"
