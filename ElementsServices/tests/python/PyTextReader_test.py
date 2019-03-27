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
from __future__ import print_function

import os
import py.test


from ElementsServices.DataSync import DataSync

from fixtures.PyTextReader import PyTextReader
from fixtures.ConfigFilesFixture import *


class TestPyTextReader(object):

    @classmethod
    def setup_class(cls):
        """The 'setup' method specifies what should be done
        prior to running the tests, i.e., in our case,
        downloading the required test files from the distant host.
        The configuration files are specified
        in the fixture constructor below.
        """

        # Let's declare the connection configuration file: sync_fr_webdav.conf
        # and the list of required test files: test_file_list.txt
        cls.sync = DataSync(
            "ElementsServices/testdata/sync_fr_webdav.conf",
            "ElementsServices/testdata/test_file_list.txt")
        cls.sync.download()

    def testReadLines(self):
        """Test reader on a local temporary file.
        """

        # Dummy file name
        filename = "/tmp/testfile.txt"

        # Dummy lines
        line = "test line"
        line2 = "YATL"

        # Write dummy file
        with open(filename, "w+") as f:
            print(line, file=f)
            print(line2, file=f)

        # Read dummy file
        reader = PyTextReader(filename)
        readLine = reader.readLine().replace('\n', '')
        readLines = reader.readLines(2)

        # Check read lines are correct
        assert readLine == line, "Misread first line: " + readLine
        assert len(readLines) == 2, "Wrong line number: " + str(len(readLines))
        assert readLines[0] == line, "Misread first line: " + readLines[0]
        assert readLines[1] == line2, "Misread second line: " + readLines[1]

        # Remove dummy file
        os.remove(filename)

    def testSynchronization(self):
        """Test reader on distant files.
        """

        for filename in theLocalFiles():
            absPath = self.sync.absolutePath(filename)
            reader = PyTextReader(absPath)
            assert reader.readLine(), "Can't read file: " + absPath
