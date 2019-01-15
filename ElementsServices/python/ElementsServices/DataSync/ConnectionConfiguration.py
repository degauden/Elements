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

import argparse
#TODO use configparse
import enum

from ElementsServices import DataSync
from .DataSyncUtils import *


class DataHost (enum.Enum):
    """The test data hosting solution.
    """
    IRODS = "iRODS"
    WEBDAV = "WebDAV"
    DSS = "DSS"


class OverwritingPolicy (enum.IntEnum):
    """The overwriting policy if the local file already exists.
    """
    ABORT = 0
    OVERWRITE = 1


class ConnectionConfiguration (object):
    """The connection configuration mainly holds:
    * the host type and URL,
    * the user name and password,
    * the overwriting policy.
    """

    def __init__ (self, filename:str=None):
        """Create a connection configuration,
        and optionally parse a connection configuration file.
        """
        self.host = None
        self.hostURL = None
        self.user = None
        self.password = None
        self.overwritingPolicy = False
        self.tries = 4
        self.distantRoot = ""
        self.localRoot = ""
        if filename is not None:
            self.parseConfigurationFile(filename)

    def overwritingAllowed (self):
        """Check whether overwriting a local file is allowed.
        """
        return self.overwritingPolicy == OverwritingPolicy.OVERWRITE

    def parseConfigurationFile (self, filename:str):
        """Parse the connection configuration file.
        """
        assert isinstance(filename, str)
        configFile = dataSyncConfFilePath(filename)
        parser = argparse.ArgumentParser()
        #TODO: use configparser
        parser.add_argument("--host", type=str)
        parser.add_argument("--host-url", type=str)
        parser.add_argument("--user", type=str)
        parser.add_argument("--password", type=str)
        parser.add_argument("--overwrite", type=str)
        parser.add_argument("--tries", type=int)
        parser.add_argument("--distant-workspace", type=str)
        parser.add_argument("--local-workspace", type=str)
        # Read config file
        values = []
        with open(configFile) as f:
            for line in f:
                if not line == '\n':
                    values.append("--" + line.replace('\n', '').replace(' ', ''))
                    #TODO avoid loop
        args = parser.parse_args(values)
        # Configure object
        self.parseHost(args.host)
        self.hostURL = args.host_url
        self.user = args.user
        self.password = args.password
        self.parseOverwritingPolicy(args.overwrite)
        self.tries = args.tries
        self.distantRoot = args.distant_workspace
        prefix = localWorkspacePrefix()
        workspace = args.local_workspace
        self.localRoot = concatenatePaths([prefix, workspace])

    def parseHost (self, name:str):
        """Parse the name of a data hosting solution (case-insensitive).
        """
        for host in DataHost:
            if host.name.lower() == name.lower():
                self.host = host
                return
        #TODO raise error

    def parseOverwritingPolicy (self, policy:str):
        overwriteAllowedOptions = ["true", "yes", "y"]
        overwriteForbiddenOptions = ["false", "no", "n"]
        if policy.lower() in overwriteAllowedOptions:
            self.overwritingPolicy = OverwritingPolicy.OVERWRITE
        elif policy.lower() in overwriteForbiddenOptions:
            self.overwritingPolicy = OverwritingPolicy.ABORT
        else:
            return
            #TODO raise error
