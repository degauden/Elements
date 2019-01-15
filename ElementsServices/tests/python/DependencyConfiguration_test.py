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

from ElementsServices.DataSync import DataSync
from ElementsServices.DataSync import DependencyConfiguration
from ElementsServices.DataSync.DataSyncUtils import getConfigurationPath, localWorkspacePrefix

from fixtures.ConfigFilesFixture import *


class TestDependencyConfiguration(object):

    def test_parseLineWithoutAlias(self):
        distantRoot = "distant/"
        localRoot = "local/"
        config = DependencyConfiguration(distantRoot, localRoot)
        file = "test/file.fits"
        assert not config.lineHasAlias(file)
        config.parseLineWithoutAlias(file)
        assert config.getDependencyCount() == 1
        assert config.getDistantPaths()[0] == distantRoot + file
        absLocalFile = concatenatePaths([localWorkspacePrefix(), localRoot, file])
        assert config.getLocalPaths()[0] == absLocalFile
        assert config.getDistantPathOf(absLocalFile) == distantRoot + file

    def test_parseLineWithAlias(self):
        distantRoot = "distant/"
        localRoot = "local/"
        config = DependencyConfiguration(distantRoot, localRoot)
        distant = "test/file_v1.fits"
        local = "test/file.fits"
        line = distant + config.getAliasSeparator() + local
        assert config.lineHasAlias(line)
        config.parseLineWithAlias(line)
        assert config.getDependencyCount() == 1
        assert config.getDistantPaths()[0] == distantRoot + distant
        absLocalFile = concatenatePaths([localWorkspacePrefix(), localRoot, local])
        assert config.getLocalPaths()[0] == absLocalFile
        assert config.getDistantPathOf(absLocalFile) == distantRoot + distant

    def test_confDependencies(self):
        theConfigFile = getConfigurationPath(theDependencyConfig())
        dependencies = DependencyConfiguration(
            "", thePrefixedLocalWorkspace(),
            theConfigFile)
        parsedFiles = dependencies.getLocalPaths()
        assert len(parsedFiles) == len(theLocalFiles()), "Input cardinal is different from read one"
        theLocalAbsPaths = [
            concatenatePaths([thePrefixedLocalWorkspace(), f])
            for f in theLocalFiles()]
        for f in theLocalAbsPaths:
            assert f in parsedFiles, "Input file not read"
        for f in parsedFiles:
            assert f in theLocalAbsPaths, "File not in input list"
