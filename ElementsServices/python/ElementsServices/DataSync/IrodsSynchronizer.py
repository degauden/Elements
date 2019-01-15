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


from .DataSynchronizer import *


def irodsIsInstalled ():
    """Check whether an iRODS client is installed.
    """
    try:
        subprocess.check_call(shlex.split("iget --help"))
        return True
    except OSError:
        return False


class IrodsSynchronizer (DataSynchronizer):
    """A data synchronizer for iRods hosts.
    """

    def __init__ (self,
            connection:ConnectionConfiguration,
            dependencies:DependencyConfiguration):
        super().__init__(connection, dependencies)
        if not irodsIsInstalled ():
            raise RuntimeError(
                'You are trying to use iRODS, '
                'but it does not seem to be installed.')

    def createDownloadCommand (self,
            distantFile:str,
            localFile:str):
        if not irodsIsInstalled ():
            return ""
        cmd = "irsync i:"
        cmd += distantFile + " " + localFile
        # Number of retries cannot be set with irsync
        # -s option allows comparing only file size (no checksum) to save time
        return cmd;
