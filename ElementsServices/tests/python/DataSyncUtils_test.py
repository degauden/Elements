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

from ElementsServices.DataSync import DataSyncUtils


class TestDataSyncUtils(object):

    def test_configFileFound(self):
        relPathToSomeFile = "ElementsServices/testdata/test_file_list.txt"
        absPathToSomeFile = DataSyncUtils.dataSyncConfFilePath(
            relPathToSomeFile)
        assert os.path.isfile(absPathToSomeFile)

    def test_runCommandAndCaptureOutErr(self):
        theMessage = "Hello, Antoine!"
        out, err = DataSyncUtils.runCommandAndCaptureOutErr(
            "echo " + theMessage)
        out = out.replace("\n", "")
        err = err.replace("\n", "")
        assert out == theMessage, "Command output misread: " + out + " != " + theMessage
        assert not err

    def test_createLocalDirOf(self):
        theDir = "/tmp/somewhere/some_temporary_test_data"
        theFilename = "some_test_file.txt"
        thePath = DataSyncUtils.concatenatePaths([DataSyncUtils.localWorkspacePrefix(), 
                                                  theDir, theFilename])
        DataSyncUtils.createLocalDirOf(thePath)
        assert os.path.isdir(DataSyncUtils.concatenatePaths([DataSyncUtils.localWorkspacePrefix(), 
                                                            theDir]))
        os.rmdir(DataSyncUtils.concatenatePaths([DataSyncUtils.localWorkspacePrefix(), 
                                                theDir]))

    def test_localWorkspacePrefix(self):
        prefixEv = DataSyncUtils.environmentVariable('NOPREFIX')
        workspaceEv = DataSyncUtils.environmentVariable('WORKSPACE')
        prefix = DataSyncUtils.localWorkspacePrefix()
        if prefixEv == '':
            assert prefix == workspaceEv
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
