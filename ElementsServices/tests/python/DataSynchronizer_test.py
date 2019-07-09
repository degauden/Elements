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
import py.test

from ElementsKernel.Temporary import TempDir, TempEnv
from ElementsServices.DataSync import DataSynchronizer

from fixtures.ConfigFilesFixture import *
from fixtures.MockDataSynchronizer import *


class TestDataSynchronizer(unittest.TestCase):

    def setUp(self):
        unittest.TestCase.setUp(self)
        self.m_top_dir = TempDir(prefix="DataSync_test")
        self.m_env = TempEnv()
        self.m_env["WORKSPACE"] = os.path.join(self.m_top_dir.path(), "workspace")
        
    def tearDown(self):
        unittest.TestCase.tearDown(self)
        del self.m_top_dir

    def testDownloadFailure(self):
        mock = MockDataSynchronizer()
        with py.test.raises(DownloadFailed):
            mock.downloadAllFiles()

    def testOverwritingPolicy(self):
        theFilename = "SayThisFileHasAlreadyBeenDownloaded.txt"
        with open(theFilename, "w") as f:
            f.write("Some contents")
        mockOverwrite = MockDataSynchronizer()
        mockNoOverwrite = MockDataSynchronizer(
            connection=theNoOverwriteConfig())
        assert mockOverwrite.fileAlreadyExists(theFilename)
        assert mockOverwrite.fileShouldBeWritten(theFilename)
        assert mockNoOverwrite.fileAlreadyExists(theFilename)
        assert not mockNoOverwrite.fileShouldBeWritten(theFilename)
        os.remove(theFilename)
