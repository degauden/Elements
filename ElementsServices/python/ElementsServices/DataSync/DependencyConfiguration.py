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


class DependencyConfiguration (object):
    """The dependency configurations holds,
    for each test file to be retrieved:
    * the distant source path,
    * the local destination path.
    """

    def __init__ (self, distantRoot:str, localRoot:str, configFile:str=None):
        """Create a dependency configuration,
        and optionally parse a dependency configuration file.
        """
        assert isinstance(distantRoot, str)
        assert isinstance(localRoot, str)
        self._distantRoot = distantRoot
        self._localRoot = os.path.join(localWorkspacePrefix(), localRoot)
        self._fileMap = {}
        self._aliasSeparator = '\t'
        if configFile is not None:
            self.parseConfigurationFile(dataSyncConfFilePath(configFile))

    def parseConfigurationFile (self, filename:str):
        """Parse a dependency configuration file.
        """
        assert isinstance(filename, str)
        with open(filename) as configFile:
            for configLine in configFile:
                stripedLine = configLine.strip().replace('\n', '').replace(' ', '')
                if stripedLine:
                    self.parseConfigurationLine(stripedLine)

    def parseConfigurationLine (self, line:str):
        """Parse a line of the dependency configuration file.
        """
        line = line.replace('\n', '')
        #TODO see Python str methods instead
        if self.lineHasAlias(line):
            self.parseLineWithAlias(line)
        else:
            self.parseLineWithoutAlias(line)

    def getAliasSeparator (self):
        """Get the separator between the filename and its alias.
        """
        return self._aliasSeparator

    def lineHasAlias (self, line:str):
        """Check whether a line of the dependency configuration file
        contains an alias.
        """
        splitLine = line.split(self._aliasSeparator)
        return len(splitLine)>1

    def parseLineWithAlias (self, line:str):
        """Parse a line of the dependency configuration file
        which contains an alias.
        """
        distant, local = line.split(self._aliasSeparator)
        distant = os.path.join(self._distantRoot, distant)
        local = os.path.join(self._localRoot, local)
        self._fileMap[local] = distant

    def parseLineWithoutAlias (self, line:str):
        """Parse a line of the dependency configuration file
        which does not contain an alias.
        """
        distant = os.path.join(self._distantRoot, line)
        local = os.path.join(self._localRoot, line)
        self._fileMap[local] = distant

    def getFileMap (self):
        """Get the local-distant file map.
        """
        return self._fileMap

    def getDistantPathOf (self, localFile:str):
        """Get the distant source path associated to a local destination path.
        """
        return self._fileMap[localFile]

    def getDependencyCount (self):
        """Get the number of files to be downloaded.
        """
        return len(self._fileMap)

    def getDistantPaths (self):
        """Get the list of all distant source paths.
        """
        return list(self._fileMap.values())

    def getLocalPaths (self):
        """Get the list of all local destination paths.
        """
        return list(self._fileMap.keys())
