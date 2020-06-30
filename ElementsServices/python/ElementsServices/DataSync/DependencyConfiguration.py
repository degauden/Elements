""" DependencyConfiguration Module """

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

from .DataSyncUtils import dataSyncConfFilePath, localWorkspacePrefix


class DependencyConfiguration(object):
    """The dependency configurations holds,
    for each test file to be retrieved:
    * the distant source path,
    * the local destination path.
    """

    def __init__(self, distant_root, local_root, config_file=None):
        """Create a dependency configuration,
        and optionally parse a dependency configuration file.
        """
        assert isinstance(distant_root, str)
        assert isinstance(local_root, str)
        self._distant_root = distant_root
        self._local_root = os.path.join(localWorkspacePrefix(), local_root)
        self._file_map = {}
        self._alias_separator = '\t'
        if config_file is not None:
            self.parseConfigurationFile(dataSyncConfFilePath(config_file))

    def parseConfigurationFile(self, filename):
        """Parse a dependency configuration file.
        """
        assert isinstance(filename, str)
        with open(filename) as config_file:
            for config_line in config_file:
                striped_line = config_line.strip().replace('\n', '').replace(' ', '')
                if striped_line:
                    self.parseConfigurationLine(striped_line)

    def parseConfigurationLine(self, line):
        """Parse a line of the dependency configuration file.
        """
        line = line.replace('\n', '')
        # TODO: see Python str methods instead
        if self.lineHasAlias(line):
            self.parseLineWithAlias(line)
        else:
            self.parseLineWithoutAlias(line)

    def getAliasSeparator(self):
        """Get the separator between the filename and its alias.
        """
        return self._alias_separator

    def lineHasAlias(self, line):
        """Check whether a line of the dependency configuration file
        contains an alias.
        """
        split_line = line.split(self._alias_separator)
        return len(split_line) > 1

    def parseLineWithAlias(self, line):
        """Parse a line of the dependency configuration file
        which contains an alias.
        """
        distant, local = line.split(self._alias_separator)
        distant = os.path.join(self._distant_root, distant)
        local = os.path.join(self._local_root, local)
        self._file_map[local] = distant

    def parseLineWithoutAlias(self, line):
        """Parse a line of the dependency configuration file
        which does not contain an alias.
        """
        distant = os.path.join(self._distant_root, line)
        local = os.path.join(self._local_root, line)
        self._file_map[local] = distant

    def getFileMap(self):
        """Get the local-distant file map.
        """
        return self._file_map

    def getDistantPathOf(self, local_file):
        """Get the distant source path associated to a local destination path.
        """
        return self._file_map[local_file]

    def getDependencyCount(self):
        """Get the number of files to be downloaded.
        """
        return len(self._file_map)

    def getDistantPaths(self):
        """Get the list of all distant source paths.
        """
        return list(self._file_map.values())

    def getLocalPaths(self):
        """Get the list of all local destination paths.
        """
        return list(self._file_map.keys())
