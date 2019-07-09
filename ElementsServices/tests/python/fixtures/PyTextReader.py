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


class PyTextReader (object):
    """Reader for simple text files.
    """

    def __init__(self, filename):
        """Create a reader for a given file.
        """
        name, extension = os.path.splitext(filename)
        assert extension.lower() == ".txt", "Only '.txt' files can be read"
        self._filename = filename

    def readLines(self, n):
        """Read up to the n-th line.
        """

        lines = []
        with open(self._filename) as f:
            lines = f.read().splitlines()

        return lines

    def readLine(self):
        """Read the first line.
        """
        return self.readLines(1)[0]
