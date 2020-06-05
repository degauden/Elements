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

import subprocess

from .DataSynchronizer import *


def webdavIsInstalled ():
    """Check whether an iRODS client is installed.
    """
    try:
        subprocess.check_call(shlex.split("wget -h"))
        return True
    except OSError:
        return False


class WebdavSynchronizer (DataSynchronizer):
    """A data synchronizer for WebDAV hosts.
    """

    def __init__ (self,
            connection,
            dependencies):
        super(WebdavSynchronizer, self).__init__(connection, dependencies)

    def createDownloadCommand (self,
            distant_file,
            local_file):
        user = self._connection.user
        password = self._connection.password
        hostURL = self._connection.host_url
        cmd = "wget --no-check-certificate "
        cmd += " --user=" + str(user)
        cmd += " --password=" + str(password)
        cmd += " -O " + local_file + " " + str(hostURL) + "/" + distant_file
        cmd += " --tries " + str(self._connection.tries)
        return cmd
