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
import os

from ElementsKernel.Temporary import TempDir, TempEnv
from ElementsServices.DataSync.DataSynchronizer import DownloadFailed

from fixtures.MockDataSynchronizer import MockDataSynchronizer
from fixtures.ConfigFilesFixture import theNoOverwriteConfig


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
        the_filename = "SayThisFileHasAlreadyBeenDownloaded.txt"
        with open(the_filename, "w") as f:
            f.write("Some contents")
        mock_overwrite = MockDataSynchronizer()
        mock_no_overwrite = MockDataSynchronizer(
            connection=theNoOverwriteConfig())
        assert mock_overwrite.fileAlreadyExists(the_filename)
        assert mock_overwrite.fileShouldBeWritten(the_filename)
        assert mock_no_overwrite.fileAlreadyExists(the_filename)
        assert not mock_no_overwrite.fileShouldBeWritten(the_filename)
        os.remove(the_filename)
