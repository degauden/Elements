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
import unittest

from ElementsKernel.Temporary import TempDir, TempEnv
from ElementsServices.DataSync import DataSyncUtils


class TestDataSyncUtils(unittest.TestCase):

    def setUp(self):
        unittest.TestCase.setUp(self)
        self.m_top_dir = TempDir(prefix="DataSync_test")
        self.m_env = TempEnv()
        self.m_env["WORKSPACE"] = os.path.join(self.m_top_dir.path(), "workspace")
        
    def tearDown(self):
        unittest.TestCase.tearDown(self)
        del self.m_top_dir

    def test_configFileFound(self):
        rel_path_to_some_file = "ElementsServices/testdata/test_file_list.txt"
        abs_path_to_some_file = DataSyncUtils.dataSyncConfFilePath(
            rel_path_to_some_file)
        assert os.path.isfile(abs_path_to_some_file)

    def test_runCommandAndCaptureOutErr(self):
        the_message = "Hello, Antoine!"
        out, err = DataSyncUtils.runCommandAndCaptureOutErr(
            "echo " + the_message)
        out = out.replace("\n", "")
        err = err.replace("\n", "")
        assert out == the_message, "Command output misread: " + out + " != " + the_message
        assert not err

    def test_createLocalDirOf(self):
        the_dir = "/tmp/somewhere/some_temporary_test_data"
        the_filename = "some_test_file.txt"
        the_path = DataSyncUtils.concatenatePaths([DataSyncUtils.localWorkspacePrefix(), 
                                                  the_dir, the_filename])
        DataSyncUtils.createLocalDirOf(the_path)
        assert os.path.isdir(DataSyncUtils.concatenatePaths([DataSyncUtils.localWorkspacePrefix(), 
                                                            the_dir]))
        os.rmdir(DataSyncUtils.concatenatePaths([DataSyncUtils.localWorkspacePrefix(), 
                                                the_dir]))

    def test_localWorkspacePrefix(self):
        prefix_ev = DataSyncUtils.environmentVariable('NOPREFIX')
        workspace_ev = DataSyncUtils.environmentVariable('WORKSPACE')
        prefix = DataSyncUtils.localWorkspacePrefix()
        if prefix_ev == '':
            assert prefix == workspace_ev
        else:
            assert prefix == ''

    def checkConcatenatePaths(self, chunks, expected):
        found = DataSyncUtils.concatenatePaths(chunks)
        assert found == expected

    def test_concatenatePaths(self):
        self.checkConcatenatePaths(["", "/tmp"], "/tmp")
        self.checkConcatenatePaths(["", "/tmp/"], "/tmp")
        self.checkConcatenatePaths(["", "/tmp", "file.fits"], "/tmp/file.fits")
        self.checkConcatenatePaths(
            ["", "/tmp/", "/data/file.fits"], "/tmp/data/file.fits")
        self.checkConcatenatePaths(["/prefix", "/tmp"], "/prefix/tmp")
