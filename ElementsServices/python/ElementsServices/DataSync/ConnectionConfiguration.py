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

""" The ConnectionConfiguration Module """

import argparse
# TODO use configparse
import enum

from .DataSyncUtils import dataSyncConfFilePath, localWorkspacePrefix, concatenatePaths


class DataHost(enum.Enum):
    """The test data hosting solution.
    """
    IRODS = "iRODS"
    WEBDAV = "WebDAV"


class OverwritingPolicy(enum.IntEnum):
    """The overwriting policy if the local file already exists.
    """
    ABORT = 0
    OVERWRITE = 1


class ConnectionConfiguration(object):
    """The connection configuration mainly holds:
    * the host type and URL,
    * the user name and password,
    * the overwriting policy.
    """

    def __init__(self, filename=None):
        """Create a connection configuration,
        and optionally parse a connection configuration file.
        """
        self.host = None
        self.host_url = None
        self.user = None
        self.password = None
        self.overwriting_policy = False
        self.tries = 4
        self.distant_root = ""
        self.local_root = ""
        if filename is not None:
            self.parseConfigurationFile(filename)

    def overwritingAllowed(self):
        """Check whether overwriting a local file is allowed.
        """
        return self.overwriting_policy == OverwritingPolicy.OVERWRITE

    def parseConfigurationFile(self, filename):
        """Parse the connection configuration file.
        """
        assert isinstance(filename, str)
        config_file = dataSyncConfFilePath(filename)
        parser = argparse.ArgumentParser()
        # TODO: use configparser
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
        with open(config_file) as f:
            for line in f:
                if not line == '\n':
                    values.append("--" + line.replace('\n', '').replace(' ', ''))
                    # TODO: avoid loop
        args = parser.parse_args(values)
        # Configure object
        self.parseHost(args.host)
        self.host_url = args.host_url
        self.user = args.user
        self.password = args.password
        self.parseOverwritingPolicy(args.overwrite)
        self.tries = args.tries
        self.distant_root = args.distant_workspace
        prefix = localWorkspacePrefix()
        workspace = args.local_workspace
        self.local_root = concatenatePaths([prefix, workspace])

    def parseHost(self, name):
        """Parse the name of a data hosting solution (case-insensitive).
        """
        for host in DataHost:
            if host.name.lower() == name.lower():
                self.host = host
                return
        # TODO raise error

    def parseOverwritingPolicy(self, policy):
        """ Parse the Overwriting Policy """
        overwrite_allowed_options = ["true", "yes", "y"]
        overwrite_forbidden_options = ["false", "no", "n"]
        if policy.lower() in overwrite_allowed_options:
            self.overwriting_policy = OverwritingPolicy.OVERWRITE
        elif policy.lower() in overwrite_forbidden_options:
            self.overwriting_policy = OverwritingPolicy.ABORT
        else:
            return
            # TODO: raise error

